//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_DECORATOR_HPP
#define MINECRAFT_DECORATOR_HPP

#include "util/math/noise/PerlinNoise.hpp"
#include "world/chunk/Chunk.hpp"

class Decorator {
 protected:
  using BlockPosT = coordinate::BlockPositionT;
  using SerialT = block::ID::SerialIDT;

 public:
  virtual void decorate(
      SerialT blockBuffer[chunk::ChunkGenSettings::CHUNK_WIDTH]
                         [chunk::ChunkGenSettings::CHUNK_HEIGHT],
      BlockPosT chunkBlockPos,
      BlockPosT blockPosX,
      BlockPosT blockPosZ,
      BlockPosT worldPosX) = 0;

 private:
};

#endif  // MINECRAFT_DECORATOR_HPP
