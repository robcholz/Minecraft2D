cmake_minimum_required(VERSION 3.24)
project(runcraft)

set(CMAKE_CXX_STANDARD 20)

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
        runcraft/client/Menu.hpp
        runcraft/client/render/RenderSystem.hpp
        runcraft/util/math/MathHelper.hpp
        runcraft/util/GameLogger.hpp
        runcraft/util/Utils.hpp
        runcraft/block/attributes/Blocks.hpp
        runcraft/block/attributes/Block.hpp
        runcraft/resource/FileHelper.hpp
        runcraft/events/SystemEvents.hpp
        runcraft/error/ErrorHandling.hpp
        runcraft/world/poi/Coordinate.hpp
        runcraft/world/chunk/Chunk.hpp
        runcraft/block/GrassBlock.hpp
        runcraft/world/poi/Position.hpp
        runcraft/block/attributes/BlockTextureLoader.hpp
        runcraft/block/AirBlock.hpp
        runcraft/block/DirtBlock.hpp
        runcraft/entity/player/PlayerEntity.hpp
        runcraft/client/scene/SceneManager.hpp
        runcraft/client/scene/SceneAccess.hpp
        runcraft/world/gen/WorldGeneration.hpp
        runcraft/world/chunk/ChunkStream.hpp
        runcraft/world/World.hpp
        runcraft/world/storage/SaveHelper.hpp
        runcraft/world/chunk/ChunkDataPacketAdapter.hpp
        runcraft/client/input/Input.hpp
        runcraft/block/IronBlock.hpp
        runcraft/block/attributes/BlockIDLoader.hpp
        runcraft/block/attributes/BlockState.hpp
        runcraft/world/poi/Direction.hpp
        runcraft/block/BirchLogBlock.hpp
        runcraft/block/attributes/BlockSoundGroupLoader.hpp
        runcraft/sound/SoundEvent.hpp
        runcraft/entity/Entity.hpp
        runcraft/world/WorldAccess.hpp 
        runcraft/world/chunk/ChunkStreamAccess.hpp
        runcraft/block/attributes/BlockAccess.hpp
        runcraft/block/ErrorBlock.hpp 
        runcraft/util/Hitbox.hpp 
        runcraft/client/gui/hud/InGameBarHud.hpp
        runcraft/client/RuncraftClient.hpp
        runcraft/entity/EntityAccess.hpp 
        runcraft/client/toast/ToastAccess.hpp 
        runcraft/client/toast/ToastManager.hpp 
        runcraft/client/toast/SystemToast.hpp 
        runcraft/client/toast/AdvancementToast.hpp 
        runcraft/client/gui/hud/HungerBarHud.hpp 
        runcraft/util/Registry.hpp 
        runcraft/util/RegistryAccess.hpp
        runcraft/sound/SoundEvents.hpp
        runcraft/sound/SoundManager.hpp
        runcraft/sound/BlockSoundGroup.hpp
        runcraft/sound/MusicSoundEvent.hpp
        runcraft/util/Identifier.hpp
        runcraft/client/RuncraftClientAccess.hpp runcraft/util/math/noise/PerlinNoise.hpp runcraft/util/math/noise/SimplexNoise.hpp runcraft/util/math/Random.hpp runcraft/block/StoneBlock.hpp runcraft/block/BedrockBlock.hpp runcraft/block/DeepslateBlock.hpp runcraft/util/ppm.hpp runcraft/world/gen/carver/Carver.hpp runcraft/block/GraniteBlock.hpp runcraft/block/DioriteBlock.hpp runcraft/block/AndesiteBlock.hpp runcraft/world/gen/feature/OreFeature.hpp runcraft/world/gen/decorator/Decorator.hpp runcraft/world/gen/decorator/HeightDecorator.hpp runcraft/client/gui/widget/TextureWidget.hpp runcraft/client/gui/widget/SplashingTextFieldWidget.hpp)

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
        test/test_bitsery/BitseryTest.cpp
        test/test_bitsery/BitseryStreamTest.cpp
        test/test_block/BlockIDTest.cpp
        test/test_util/IdentifierTest.cpp
        test/test_math/MathTest.cpp test/test_world_gen/CaveTest.cpp)
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

