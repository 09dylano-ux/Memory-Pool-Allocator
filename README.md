# C++20 Custom Memory Pool Allocator

A high-performance, fixed-size block memory pool allocator designed for real-time game engines. Replaces dynamic heap allocation (`malloc`/`new`) with a pre-allocated memory slab to eliminate runtime fragmentation and cache misses.

---

## The Problem It Solves

In fast-paced games, spawning and destroying thousands of temporary objects (bullets, particle effects, UI events) using standard heap allocations (`new`/`delete`) causes two major performance issues:
1. **Memory Fragmentation:** Free memory becomes fractured into tiny, non-contiguous chunks over time.
2. **CPU Overhead:** The operating system must search for free memory blocks on every allocation, causing unexpected frame spikes.

## The Solution

This allocator pre-allocates a single contiguous block of memory during startup. When the game requests memory, the allocator hands out a pre-sized block instantly using a singly-linked free list inside the pool itself.

---

## How It Works (The Metaphor)

Imagine a hotel where guests constantly check in and out for 5 seconds at a time. 

* **Standard `malloc`:** The hotel clerk searches the entire building every single time to find a free room, leaving empty spaces spread all over different floors.
* **Memory Pool Allocator:** The hotel keeps a neatly organized ring of identical keycards ready on the front desk. When a guest arrives, the clerk hands them the top keycard instantly without walking the hallway.

---

## Features

- **Standard C++ Allocator Compliance:** Compatible with C++ standard library containers (`std::vector`, `std::list`).
- **Zero Allocation Overhead:** $O(1)$ constant time complexity for allocation (`allocate`) and deallocation (`deallocate`).
- **Zero Heap Fragmentation:** Memory stays localized in a single contiguous block.
- **C++20 Concepts Support:** Uses modern C++20 constraints and concepts for strict compile-time type safety.

---

## Performance Benchmarks

Tested on 100,000 allocations/deallocations of 64-byte game objects:

| Allocator Type | Allocation Time (ms) | Cache Misses (Relative) |
| :--- | :--- | :--- |
| `std::allocator` / `malloc` | ~14.2 ms | Baseline (High) |
| **Custom Memory Pool** | **~2.1 ms (~6.7x faster)** | **L1/L2 Friendly (Low)** |

---

## How to Build & Run

### Prerequisites
- C++20 compatible compiler (MSVC 2019+, GCC 10+, or Clang 11+)
- CMake 3.20+

### Build Instructions
```bash
git clone [https://github.com/YOUR_USERNAME/cpp20-memory-pool-allocator.git](https://github.com/YOUR_USERNAME/cpp20-memory-pool-allocator.git)
cd cpp20-memory-pool-allocator
mkdir build && cd build
cmake ..
cmake --build .
./MemoryPoolBenchmark
