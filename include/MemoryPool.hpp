#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <cstddef>
#include <concepts>
#include <utility>
#include <new>
#include <cstdint>

template <typename T>
concept AllocatableType = !std::is_reference_v<T> && (sizeof(T) >= sizeof(void*));

template <typename T, std::size_t BlockCount = 1024>
requires AllocatableType<T>
class MemoryPool {
private:
    union Node {
        Node* next;
        alignas(alignof(T)) char storage[sizeof(T)];
    };

    Node* m_poolMemory;
    Node* m_freeListHeader;

public:
    using value_type = T;

    explicit MemoryPool() : m_poolMemory(nullptr), m_freeListHeader(nullptr) {
        m_poolMemory = static_cast<Node*>(::operator new[](sizeof(Node) * BlockCount));
        
        for (std::size_t i = 0; i < BlockCount - 1; ++i) {
            m_poolMemory[i].next = &m_poolMemory[i + 1];
        }
        m_poolMemory[BlockCount - 1].next = nullptr;
        m_freeListHeader = &m_poolMemory[0];
    }

    ~MemoryPool() noexcept {
        ::operator delete[](m_poolMemory);
    }

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    MemoryPool(MemoryPool&& other) noexcept 
        : m_poolMemory(other.m_poolMemory), m_freeListHeader(other.m_freeListHeader) {
        other.m_poolMemory = nullptr;
        other.m_freeListHeader = nullptr;
    }

    MemoryPool& operator=(MemoryPool&& other) noexcept {
        if (this != &other) {
            if (m_poolMemory) {
                ::operator delete[](m_poolMemory);
            }
            m_poolMemory = other.m_poolMemory;
            m_freeListHeader = other.m_freeListHeader;
            other.m_poolMemory = nullptr;
            other.m_freeListHeader = nullptr;
        }
        return *this;
    }

    [[nodiscard]] T* allocate() {
        if (m_freeListHeader == nullptr) {
            throw std::bad_alloc();
        }

        Node* allocatedNode = m_freeListHeader;
        m_freeListHeader = m_freeListHeader->next;

        return reinterpret_cast<T*>(allocatedNode->storage);
    }

    void deallocate(T* ptr) noexcept {
        if (ptr == nullptr) return;

        Node* node = reinterpret_cast<Node*>(ptr);
        node->next = m_freeListHeader;
        m_freeListHeader = node;
    }

    template <typename... Args>
    T* construct(Args&&... args) {
        T* memory = allocate();
        ::new (static_cast<void*>(memory)) T(std::forward<Args>(args)...);
        return memory;
    }

    void destroy(T* ptr) noexcept {
        if (ptr != nullptr) {
            ptr->~T();
            deallocate(ptr);
        }
    }

    [[nodiscard]] constexpr std::size_t capacity() const noexcept {
        return BlockCount;
    }
};

#endif // MEMORY_POOL_HPP
