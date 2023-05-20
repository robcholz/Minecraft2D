cmake_minimum_required(VERSION 3.24)
project(runcraft)

set(CMAKE_CXX_STANDARD 17)

# source files
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
        runcraft/sound/BlockSoundGroup.hpp
        runcraft/world/poi/WorldCoordinate.hpp
        runcraft/world/chunk/Chunk.hpp
        runcraft/block/GrassBlock.hpp
        runcraft/block/BlockPosition.hpp
        runcraft/block/BlockTextureLoader.hpp
        runcraft/block/AirBlock.hpp
        runcraft/block/DirtBlock.hpp
        runcraft/client/player/Player.hpp
        runcraft/client/scene/SceneManager.hpp
        runcraft/client/scene/Scene.hpp
        runcraft/world/gen/WorldGeneration.hpp
        runcraft/world/chunk/ChunkStream.hpp
        runcraft/world/World.hpp
        runcraft/world/storage/SaveHelper.hpp
        runcraft/world/chunk/ChunkDataPacketAdapter.hpp runcraft/client/input/Input.hpp)


#includes
add_executable(runcraft ${SOURCE_FILES})
include_directories(/usr/local/include)
include_directories(include/rapidjson)
include_directories(include/plog)
include_directories(include/json)
include_directories(include)

include_directories(runcraft)

#boost
find_package(Boost 1.81.0 REQUIRED)

#sfml
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(runcraft sfml-system sfml-window sfml-graphics sfml-audio sfml-network)


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

add_executable(
        RunCraftTest
        test/test_block/BlockTest.cpp
        test/test_resource/ResourceTest.cpp
        test/test_world/WorldGenerationTest.cpp
        test/test_bitsery/BitseryTest.cpp
        test/test_bitsery/BitseryStreamTest.cpp
        test/test_input/InputTest.cpp
)
target_link_libraries(
        RunCraftTest
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(RunCraftTest)

