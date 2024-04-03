//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_CARVER_HPP
#define MINECRAFT_CARVER_HPP

#include "block/attributes/Block.hpp"
#include "util/math/noise/PerlinNoise.hpp"
#include "world/chunk/Chunk.hpp"

class Carver {
 private:
  using ChunkPosT = coordinate::ChunkPositionT;
  using BlockPosT = coordinate::BlockPositionT;

 protected:
 public:
  explicit Carver(unsigned int seed) { noise.initialize(seed); }

  void curve(
      block::ID::SerialIDT(blockBuffer)[chunk::ChunkGenSettings::CHUNK_WIDTH]
                                       [chunk::ChunkGenSettings::CHUNK_HEIGHT],
      BlockPosT blockPosX,
      BlockPosT chunkBlockPosX,
      BlockPosT chunkBlockPosZ,
      int curveThresholdConstant,
      bool condition) {
    double x = (double)blockPosX / ((double)100);
    double y = (double)chunkBlockPosZ / ((double)100);
    double n = 2 * noise.noise(x * 5, y * 5, 0.8);
    auto grey_channel = (uint8_t)floor(255 * n);
    if (grey_channel <= curveThresholdConstant && condition)
      blockBuffer[chunkBlockPosX][chunkBlockPosZ] =
          block::Blocks::getInstance()
              .getObjectInstance("minecraft:air_block")
              ->getSerialID();
  }

  ~Carver() = default;

 private:
  math::PerlinNoise noise;
};

#endif  // MINECRAFT_CARVER_HPP
