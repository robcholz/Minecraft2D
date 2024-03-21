//
// Created by robcholz on 8/12/23.
//

#ifndef MINECRAFT_2D_WORLDCHUNKACCESS_HPP
#define MINECRAFT_2D_WORLDCHUNKACCESS_HPP

#include "ChunkStreamAccess.hpp"
#include "block/attributes/Block.hpp"

namespace chunk {
class WorldChunkAccess {
 private:
  using ChunkPosT = coordinate::ChunkPositionT;
  using BlockPosT = coordinate::BlockPositionT;

 public:
  virtual Chunk* getChunk(ChunkPosT chunkPos) = 0;

  virtual ChunkStreamAccess* getChunkStream() = 0;

  virtual uint8_t getBlockLightLevel(
      const coordinate::BlockPos& blockPosition) = 0;

  virtual uint8_t getBlockLightLevel(BlockPosT x, BlockPosT z) = 0;

  virtual block::Block* getBlock(
      const coordinate::BlockPosition& blockPosition) = 0;

  virtual block::Block* getBlock(const coordinate::BlockPos& blockPosition) = 0;

  virtual void setBlockLightLevel(BlockPosT x,
                                  BlockPosT z,
                                  uint8_t lightLevel) = 0;

  virtual void setBlockLightLevel(const coordinate::BlockPos& blockPosition,
                                  uint8_t lightLevel) = 0;
};
}  // namespace chunk

#endif  // MINECRAFT_2D_WORLDCHUNKACCESS_HPP
