project(RunCraftTests LANGUAGES CXX)

add_executable(RunCraftTests ResourceTest.cpp)

target_link_libraries(RunCraftTests PRIVATE )

add_test(NAME RunCraftTests COMMAND RunCraftTests)