cmake_minimum_required(VERSION 3.24)
project(RunCraft)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES RunCraft.cpp game/Game.hpp game/util/Math_Helper.hpp
        game/util/File.hpp game/widget/Button.hpp game/Audio.hpp
        game/widget/Texture.hpp game/Render.hpp game/widget/Widgets.hpp
        game/GameInfo.hpp game/util/utils.hpp game/util/GameLogger.hpp game/util/FileHelper.hpp)
add_executable(RunCraft ${SOURCE_FILES})
include_directories(/usr/local/include)
include_directories(include/rapidjson)
include_directories(include/plog)


find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(RunCraft sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

