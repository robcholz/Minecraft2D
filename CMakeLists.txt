cmake_minimum_required(VERSION 3.24)
project(RunCraft)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES RunCraft.cpp RunCraft/util/Math_Helper.hpp
        RunCraft/resource/File.hpp RunCraft/client/gui/widget/ButtonWidget.hpp RunCraft/sound/Audio.hpp RunCraft/client/gui/widget/texture/Texture.hpp
        RunCraft/client/render/Render.hpp RunCraft/client/GameInfo.hpp RunCraft/util/utils.hpp RunCraft/util/GameLogger.hpp
        RunCraft/resource/FileHelper.hpp RunCraft/client/gui/GUI.hpp RunCraft/client/gui/screen/Background.hpp
        RunCraft/client/gui/widget/texture/SingleTextureComponent.hpp
        RunCraft/client/gui/text/RichText.hpp RunCraft/client/gui/text/RichText.hpp RunCraft/events/SystemEvents.hpp
        RunCraft/client/gui/screen/Screen.hpp RunCraft/client/gui/widget/SliderWidget.hpp RunCraft/client/Menu.hpp RunCraft/error/ErrorHandling.hpp RunCraft/client/gui/screen/ScreenManager.hpp RunCraft/client/RunCraft.hpp RunCraft/client/gui/widget/SoundSliderWidget.hpp RunCraft/client/gui/widget/TextFieldWidget.hpp RunCraft/client/gui/style/GUIStyle.hpp RunCraft/client/gui/widget/TexturedButtonWidget.hpp RunCraft/client/gui/text/TranslatableText.hpp RunCraft/item/Item.hpp RunCraft/item/Items.hpp RunCraft/block/Blocks.hpp RunCraft/block/Block.hpp)
add_executable(RunCraft ${SOURCE_FILES})
include_directories(/usr/local/include)
include_directories(include/rapidjson)
include_directories(include/plog)
include_directories(RunCraft)
include_directories(game)


find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(RunCraft sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

