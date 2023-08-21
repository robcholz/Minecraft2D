set(SOURCE_FILES_CLIENT
        Minecraft.cpp
        client/gui/widget/TexturedButtonWidget.hpp
        client/gui/widget/SoundSliderWidget.hpp
        client/gui/widget/TextFieldWidget.hpp
        client/gui/widget/ButtonWidget.hpp
        client/gui/widget/SliderWidget.hpp
        resource/TranslatableText.hpp
        client/gui/screen/ScreenManager.hpp
        client/gui/screen/Background.hpp
        client/gui/screen/Screen.hpp
        client/gui/style/GUIStyle.hpp
        client/gui/text/RichText.hpp
        client/gui/GUI.hpp
        client/Menu.hpp
        client/render/RenderSystem.hpp
        util/math/MathHelper.hpp
        util/GameLogger.hpp
        util/Utils.hpp
        block/attributes/Blocks.hpp
        block/attributes/Block.hpp
        resource/FileHelper.hpp
        events/SystemEvents.hpp
        error/ErrorHandling.hpp
        world/poi/Coordinate.hpp
        world/chunk/Chunk.hpp
        block/GrassBlock.hpp
        world/poi/Position.hpp
        block/attributes/BlockTextureLoader.hpp
        block/AirBlock.hpp
        block/DirtBlock.hpp
        entity/player/PlayerEntity.hpp
        client/scene/SceneManager.hpp
        client/scene/SceneAccess.hpp
        world/gen/WorldGeneration.hpp
        world/chunk/ChunkStream.hpp
        world/World.hpp
        world/storage/SaveHelper.hpp
        world/chunk/ChunkDataPacketAdapter.hpp
        client/input/Input.hpp
        block/IronBlock.hpp
        block/attributes/BlockIDLoader.hpp
        block/attributes/BlockState.hpp
        world/poi/Direction.hpp
        block/BirchLogBlock.hpp
        sound/SoundEvent.hpp
        entity/Entity.hpp
        world/WorldAccess.hpp
        world/chunk/ChunkStreamAccess.hpp
        block/attributes/BlockAccess.hpp
        block/ErrorBlock.hpp
        util/Hitbox.hpp
        client/gui/hud/InGameBarHud.hpp
        client/MinecraftClient.hpp
        entity/EntityAccess.hpp
        client/toast/ToastAccess.hpp
        client/toast/ToastManager.hpp
        client/toast/SystemToast.hpp
        client/toast/AdvancementToast.hpp
        client/gui/hud/HungerBarHud.hpp
        util/Registry.hpp
        util/RegistryAccess.hpp
        sound/SoundEvents.hpp
        sound/SoundManager.hpp
        sound/BlockSoundGroup.hpp
        sound/MusicSoundEvent.hpp
        util/Identifier.hpp
        client/MinecraftClientAccess.hpp
        util/math/noise/PerlinNoise.hpp
        util/math/Random.hpp
        block/StoneBlock.hpp
        block/BedrockBlock.hpp
        block/DeepslateBlock.hpp
        util/image/ppm.hpp
        world/gen/carver/Carver.hpp
        block/GraniteBlock.hpp
        block/DioriteBlock.hpp
        block/AndesiteBlock.hpp
        world/gen/feature/OreFeature.hpp
        world/gen/decorator/Decorator.hpp
        world/gen/decorator/HeightDecorator.hpp
        client/gui/widget/TextureWidget.hpp
        client/gui/widget/SplashingTextFieldWidget.hpp
        util/Path.hpp
        resource/System.hpp
        command/CommandLineExecutable.hpp
        MinecraftVersion.hpp
        util/Date.hpp
        block/attributes/BlockSoundGroup.hpp
        client/render/TileColor.hpp
        block/GlowstoneBlock.hpp
        world/chunk/WorldChunk.hpp
        world/chunk/WorldChunkAccess.hpp
        world/chunk/light/LightingProvider.hpp
        world/chunk/light/LightingProviderAccess.hpp client/render/WorldRenderer.hpp)

add_executable(MinecraftClient ${SOURCE_FILES_CLIENT})

include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/include/bitsery)
include_directories(${CMAKE_SOURCE_DIR}/include/plog)
include_directories(${CMAKE_SOURCE_DIR}/include/json)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/minecraft)


#plist
include_directories(${CMAKE_SOURCE_DIR}/include/plist)
target_link_libraries(MinecraftClient Plist)

# mem pool
include_directories(${CMAKE_SOURCE_DIR}/include/mempool)
target_link_libraries(MinecraftClient MemPool)

#boost
find_package(Boost 1.81.0 COMPONENTS system filesystem date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(MinecraftClient ${Boost_LIBRARIES})

#sfml
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(MinecraftClient sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

