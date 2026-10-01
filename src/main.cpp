
#include "../include/MemoryPool.hpp"
#include <iostream>
#include <vector>
#include <chrono>

struct GameEntity {
    float position[3];
    float velocity[3];
    int entityId;
    char padding[32]; // Expands size to 60 bytes
};

int main() {
    constexpr std::size_t ITERATIONS = 100000;
    std::cout << "Starting Memory Allocation Performance Benchmark...\n";
    std::cout << "Testing " << ITERATIONS << " allocations/deallocations.\n\n";

    // 1. Standard Heap Allocation Benchmark
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<GameEntity*> heapPointers;
        heapPointers.reserve(ITERATIONS);

        for (std::size_t i = 0; i < ITERATIONS; ++i) {
            heapPointers.push_back(new GameEntity());
        }

        for (std::size_t i = 0; i < ITERATIONS; ++i) {
            delete heapPointers[i];
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "[Standard Heap Allocator (malloc/new)] Time: " << duration.count() << " ms\n";
    }

    // 2. Custom Memory Pool Allocator Benchmark
    {
        auto start = std::chrono::high_resolution_clock::now();
        MemoryPool<GameEntity, ITERATIONS> pool;
        std::vector<GameEntity*> poolPointers;
        poolPointers.reserve(ITERATIONS);

        for (std::size_t i = 0; i < ITERATIONS; ++i) {
            poolPointers.push_back(pool.construct());
        }

        for (std::size_t i = 0; i < ITERATIONS; ++i) {
            pool.destroy(poolPointers[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "[Custom C++20 Memory Pool Allocator] Time: " << duration.count() << " ms\n";
    }

    return 0;
}
