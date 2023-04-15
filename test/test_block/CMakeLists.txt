project(BlockTests LANGUAGES CXX)

add_executable(BlockTests BlockTest.cpp)

target_link_libraries(BlockTests PRIVATE )

add_test(NAME BlockTests COMMAND BlockTests)