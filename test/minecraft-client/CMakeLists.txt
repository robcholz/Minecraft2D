#unit test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
## For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(SOURCE_FILES_CLIENT_TEST
        test_bitsery/BitseryStreamTest.cpp
        test_bitsery/BitseryTest.cpp
        test_block/BlockIDTest.cpp
        test_math/MathTest.cpp
        test_util/IdentifierTest.cpp
        test_world_gen/CaveTest.cpp
        test_mempool/MemPoolTest.cpp
        )

add_executable(MinecraftClientTest ${SOURCE_FILES_CLIENT_TEST})

# mem pool
include_directories(${CMAKE_SOURCE_DIR}/include/mempool)
target_link_libraries(MinecraftClient MemPool)

#sfml
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(MinecraftClientTest sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

include(GoogleTest)
target_link_libraries(
        MinecraftClientTest
        GTest::gtest_main
)
gtest_discover_tests(MinecraftClientTest)

include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/include/bitsery)
include_directories(${CMAKE_SOURCE_DIR}/include/plog)
include_directories(${CMAKE_SOURCE_DIR}/include/json)
include_directories(${CMAKE_SOURCE_DIR}/include/mempool)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/minecraft)