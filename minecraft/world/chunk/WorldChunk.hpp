//
// Created by robcholz on 8/12/23.
//

#ifndef MINECRAFT_2D_WORLDCHUNK_HPP
#define MINECRAFT_2D_WORLDCHUNK_HPP

#include "ChunkStream.hpp"
#include "WorldChunkAccess.hpp"
#include "error/ErrorHandling.hpp"
#include "world/chunk/light/LightingProvider.hpp"

namespace chunk {
class WorldChunk : public WorldChunkAccess {
 private:
  using ChunkPosT = coordinate::ChunkPositionT;
  using BlockPosT = coordinate::BlockPositionT;

 public:
  WorldChunk() = delete;

  explicit WorldChunk(WorldAccess* worldAccess,
                      ChunkStream::DistanceT simulationDistance,
                      ChunkStream::DistanceT renderDistance) {
    chunkStream = std::make_unique<ChunkStream>(worldAccess, simulationDistance,
                                                renderDistance);
    lightingProvider = std::make_unique<LightingProvider>(this);
    chunkStream->onChunkRender([this](ChunkPosT chunkPos) {
      lightingProvider->updateDaylight(chunkPos);
    });
  }

  ~WorldChunk() = default;

  std::optional<std::reference_wrapper<Chunk>> getChunk(
      ChunkPosT chunkPos) override {
    return chunkStream->getChunk(chunkPos);
  }

  const ChunkStreamAccess& getChunkStream() const override {
    return *chunkStream;
  }

  uint8_t getBlockLightLevel(
      const coordinate::BlockPos& blockPosition) override {
    return getBlockLightLevel(blockPosition.x, blockPosition.z);
  }

  uint8_t getBlockLightLevel(BlockPosT x, BlockPosT z) override {
    auto chunkSettings = Chunk::toChunkPosition(x, z);
    auto chunk = this->getChunk(chunkSettings.chunkPos);
    if (chunk.has_value())
      return chunk->get().getBlockLightLevel(chunkSettings.blockPos);
    else {
      PLOG_ERROR << "no such chunk exists";
    }
    return 0;
  }

  block::Block& getBlockUnsafe(
      const coordinate::BlockPosition& blockPosition) override {
    return getBlockUnsafe(blockPosition.get());
  }

  block::Block& getBlockUnsafe(
      const coordinate::BlockPos& blockPosition) override {
    return getBlock(blockPosition).value().get();
  }

  virtual std::optional<std::reference_wrapper<block::Block>> getBlock(
      const coordinate::BlockPosition& blockPosition) override {
    return getBlock(blockPosition.get());
  }

  virtual std::optional<std::reference_wrapper<block::Block>> getBlock(
      const coordinate::BlockPos& blockPosition) override {
    // convert to chunk position and search in stream
    // locate the block pointer in a specific chunk
    auto chunkSettings =
        Chunk::toChunkPosition(blockPosition.x, blockPosition.z);
    auto chunk = this->getChunk(chunkSettings.chunkPos);
    if (chunk.has_value()) {
      return *chunk->get().getBlockWithBoundaryCheck(chunkSettings);
    }
    PLOG_VERBOSE << "Given block " << blockPosition.x << " " << blockPosition.z
                 << " doesn't exist in the loaded chunks: ChunkPosition: "
                 << chunkSettings.chunkPos;
    return std::nullopt;
  }

  void setBlockLightLevel(const BlockPosT& x,
                          const BlockPosT& z,
                          uint8_t lightLevel) override {
    auto chunkSettings = Chunk::toChunkPosition(x, z);
    auto chunk = this->getChunk(chunkSettings.chunkPos);
    if (chunk.has_value()) {
      chunk->get().setBlockLightLevel(chunkSettings.blockPos.x,
                                      chunkSettings.blockPos.z, lightLevel);
    } else {
      PLOG_ERROR << "no such chunk exists";
    }
  }

  void setBlockLightLevel(const coordinate::BlockPos& blockPosition,
                          uint8_t lightLevel) override {
    setBlockLightLevel(blockPosition.x, blockPosition.z, lightLevel);
  }

  // main chunk update function
  void update() {
    chunkStream->update();
    lightingProvider->update();
  }

  void setChunkGenerator(
      std::function<std::unique_ptr<Chunk>(ChunkPosT)> function) {
    chunkStream->setChunkGenerator(std::move(function));
  }

 private:
  std::unique_ptr<ChunkStream> chunkStream;
  std::unique_ptr<LightingProvider> lightingProvider;
};
}  // namespace chunk

#endif  // MINECRAFT_2D_WORLDCHUNK_HPP
