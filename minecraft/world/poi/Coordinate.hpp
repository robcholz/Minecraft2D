//
// Created by robcholz on 4/4/23.
//

#ifndef MINECRAFT_COORDINATE_HPP
#define MINECRAFT_COORDINATE_HPP

namespace coordinate {
using EntityPositionT = float;
using BlockPositionT = int;
using ChunkPositionT = int;
using PixelPositonT = int;

struct Acceleration {
  EntityPositionT x, z;
};

struct Velocity {
  EntityPositionT x, z;
};

struct EulerAngle {
  float x, y, z;
};

struct EntityPos {
  using type = EntityPositionT;
  type x, z;

  EntityPos(type x_, type z_) : x(x_), z(z_) {}
};

struct BlockPos {
  using type = BlockPositionT;
  type x, z;

  BlockPos(type x_, type z_) : x(x_), z(z_) {}
};

struct ChunkPos {
  using type = ChunkPositionT;
  type chunkPos = 0;
  BlockPos blockPos;
  explicit ChunkPos(type chunkPos = 0, BlockPos blockPos = BlockPos(0, 0))
      : chunkPos(chunkPos), blockPos(blockPos) {}
};

struct PixelPos {
  using type = PixelPositonT;
  type x, z;

  PixelPos(type x_, type z_) : x(x_), z(z_) {}
};
}  // namespace coordinate

#endif  // MINECRAFT_COORDINATE_HPP
