cmake_minimum_required(VERSION 3.24)
project(RunCraft)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES RunCraft.cpp Minecraft/util/Math_Helper.hpp
        Minecraft/util/File.hpp Minecraft/GUI/widget/Button.hpp Minecraft/Sound/Audio.hpp Minecraft/Render/SingleComponent/Texture.hpp
        Minecraft/Render/Render.hpp game/GameInfo.hpp Minecraft/util/utils.hpp Minecraft/util/GameLogger.hpp
        Minecraft/util/FileHelper.hpp Minecraft/GUI/GUI.hpp Minecraft/GUI/screen/Background.hpp
        Minecraft/Render/SingleComponent/SingleTextureComponent.hpp Minecraft/GUI/widget/WidgetManager.hpp
        Minecraft/GUI/text/RichText.hpp Minecraft/GUI/text/RichText.hpp Minecraft/Events/SystemEvents.hpp
        Minecraft/GUI/screen/Screen.hpp Minecraft/GUI/widget/Slider.hpp game/Menu.hpp Minecraft/ErrorHandling/ErrorHandling.hpp Minecraft/GUI/screen/ScreenManager.hpp)
add_executable(RunCraft ${SOURCE_FILES})
include_directories(/usr/local/include)
include_directories(include/rapidjson)
include_directories(include/plog)
include_directories(Minecraft)
include_directories(game)


find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(RunCraft sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

