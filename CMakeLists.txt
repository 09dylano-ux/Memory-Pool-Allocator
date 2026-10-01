cmake_minimum_required(VERSION 3.20)
project(Cpp20MemoryPoolAllocator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(MemoryPoolBenchmark src/main.cpp include/MemoryPool.hpp)
