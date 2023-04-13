cmake_minimum_required(VERSION 3.24)
project(runcraft)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        RunCraft.cpp
        runcraft/client/gui/widget/texture/SingleTextureComponent.hpp
        runcraft/client/gui/widget/TexturedButtonWidget.hpp
        runcraft/client/gui/widget/SoundSliderWidget.hpp
        runcraft/client/gui/widget/TextFieldWidget.hpp
        runcraft/client/gui/widget/texture/Texture.hpp
        runcraft/client/gui/widget/ButtonWidget.hpp
        runcraft/client/gui/widget/SliderWidget.hpp
        runcraft/resource/TranslatableText.hpp
        runcraft/client/gui/screen/ScreenManager.hpp
        runcraft/client/gui/screen/Background.hpp
        runcraft/client/gui/screen/Screen.hpp
        runcraft/client/gui/style/GUIStyle.hpp
        runcraft/client/gui/text/RichText.hpp
        runcraft/client/gui/GUI.hpp
        runcraft/client/GameInfo.hpp
        runcraft/client/RunCraft.hpp
        runcraft/client/Menu.hpp
        runcraft/client/render/Render.hpp
        runcraft/util/MathHelper.hpp
        runcraft/util/GameLogger.hpp
        runcraft/util/Utils.hpp
        runcraft/block/Blocks.hpp
        runcraft/block/Block.hpp
        runcraft/sound/Audio.hpp
        runcraft/resource/FileHelper.hpp
        runcraft/events/SystemEvents.hpp
        runcraft/error/ErrorHandling.hpp
        runcraft/sound/BlockSoundGroup.hpp runcraft/world/poi/WorldCoordinate.hpp runcraft/world/chunk/Chunk.hpp)
add_executable(runcraft ${SOURCE_FILES})
include_directories(/usr/local/include)
include_directories(include/rapidjson)
include_directories(include/plog)
include_directories(include/json)
include_directories(runcraft)
include_directories(game)


find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(runcraft sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

