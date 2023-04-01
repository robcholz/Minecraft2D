cmake_minimum_required(VERSION 3.24)
project(RunCraft)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES RunCraft.cpp RunCraft/util/Math_Helper.hpp
        RunCraft/util/File.hpp RunCraft/GUI/widget/Button.hpp RunCraft/Sound/Audio.hpp RunCraft/Render/SingleComponent/Texture.hpp
        RunCraft/Render/Render.hpp game/GameInfo.hpp RunCraft/util/utils.hpp RunCraft/util/GameLogger.hpp
        RunCraft/util/FileHelper.hpp RunCraft/GUI/GUI.hpp RunCraft/GUI/screen/Background.hpp
        RunCraft/Render/SingleComponent/SingleTextureComponent.hpp
        RunCraft/GUI/text/Text.hpp RunCraft/GUI/text/Text.hpp RunCraft/Events/SystemEvents.hpp
        RunCraft/GUI/screen/Screen.hpp RunCraft/GUI/widget/Slider.hpp game/Menu.hpp RunCraft/ErrorHandling/ErrorHandling.hpp RunCraft/GUI/screen/ScreenManager.hpp game/RunCraft.hpp RunCraft/GUI/widget/ValueSlider.hpp)
add_executable(RunCraft ${SOURCE_FILES})
include_directories(/usr/local/include)
include_directories(include/rapidjson)
include_directories(include/plog)
include_directories(RunCraft)
include_directories(game)


find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(RunCraft sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

