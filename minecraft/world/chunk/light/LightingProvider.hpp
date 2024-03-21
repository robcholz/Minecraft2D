
//
// Created by robcholz on 8/12/23.
//

#ifndef MINECRAFT_2D_LIGHTINGPROVIDER_HPP
#define MINECRAFT_2D_LIGHTINGPROVIDER_HPP

#include <queue>
#include "LightingProviderAccess.hpp"
#include "world/chunk/Chunk.hpp"
#include "world/chunk/WorldChunkAccess.hpp"

class LightingProvider : public LightingProviderAccess {
 private:
  using BlockPosT = coordinate::BlockPositionT;
  using ChunkPosT = coordinate::ChunkPositionT;

 public:
  LightingProvider() = delete;

  explicit LightingProvider(chunk::WorldChunkAccess* worldChunkAccess) {
    this->worldChunkAccess = worldChunkAccess;
  }

  LightingProvider(const LightingProvider&) = delete;

  const LightingProvider& operator=(const LightingProvider&) = delete;

  ~LightingProvider() = default;

  void updateLightSource(const coordinate::BlockPos& blockPosition,
                         uint8_t lightLevel) override {
    lightUpdatesQueue.emplace(blockPosition, lightLevel);
  }

  // exert the light due to the influence of the daylights
  void updateDaylight(ChunkPosT chunkPos) {
    for (auto x_in_chunk = 0; x_in_chunk < chunk::ChunkGenSettings::CHUNK_WIDTH;
         ++x_in_chunk) {
      // get world-x position
      auto x_world = chunk::Chunk::toBlockPosition(chunkPos, x_in_chunk, 15).x;
      // set the all the air blocks exposed to daylight to the max light level
      for (auto z_pos =
               worldChunkAccess->getChunk(chunkPos)->getHeightMap(x_in_chunk);
           z_pos < chunk::ChunkGenSettings::CHUNK_HEIGHT; ++z_pos) {
        updateLightSource({x_world, z_pos}, 15);
      }
    }  // iterate through the blocks inside the chunk
  }

  void update() {
    while (!lightUpdatesQueue.empty()) {
      auto& lightUpdate = lightUpdatesQueue.front();
      propagateLight(lightUpdate.first.x, lightUpdate.first.z,
                     lightUpdate.second);
      lightUpdatesQueue.pop();
    }
  }

 private:
  chunk::WorldChunkAccess* worldChunkAccess = nullptr;
  std::queue<std::pair<coordinate::BlockPos, uint8_t>> lightUpdatesQueue;

  /**
   * @brief propagate light
   * @param x world position
   * @param z world position
   * @param lightLevel the level of light
   */
  void propagateLight(BlockPosT x, BlockPosT z, uint8_t lightLevel) {
    if (lightLevel <= 0)
      return;
    auto chunk_pos = chunk::Chunk::toChunkPosition(x, z);
    auto render_interval =
        worldChunkAccess->getChunkStream()->getRenderInterval();
    // check if the block is in the render interval
    if (!(render_interval.lower < chunk_pos.chunkPos ||
          render_interval.upper > chunk_pos.chunkPos))
      return;
    // check if the block position is in the range
    if (chunk_pos.blockPos.x < 0 ||
        chunk_pos.blockPos.x >= chunk::ChunkGenSettings::CHUNK_WIDTH ||
        chunk_pos.blockPos.z < 0 ||
        chunk_pos.blockPos.z >= chunk::ChunkGenSettings::CHUNK_HEIGHT)
      return;
    uint8_t currentLightLevel = worldChunkAccess->getBlockLightLevel(x, z);
    if (lightLevel > currentLightLevel) {
      worldChunkAccess->setBlockLightLevel(x, z, lightLevel);
      propagateLight(x + 1, z, lightLevel - 1);
      propagateLight(x - 1, z, lightLevel - 1);
      propagateLight(x, z + 1, lightLevel - 1);
      propagateLight(x, z - 1, lightLevel - 1);
    }
  }
};

#endif  // MINECRAFT_2D_LIGHTINGPROVIDER_HPP
