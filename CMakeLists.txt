cmake_minimum_required(VERSION 3.24)
project(minecraft-2d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR /build/client)

# libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/include/plist)
add_subdirectory(${CMAKE_SOURCE_DIR}/include/mempool)

# client and server
add_subdirectory(minecraft)
add_subdirectory(test/minecraft-client)

