//
// Created by robcholz on 4/4/23.
//

#ifndef MINECRAFT_CHUNK_HPP
#define MINECRAFT_CHUNK_HPP

#include <bitsery/brief_syntax.h>
#include <algorithm>
#include <array>
#include <memory>
#include "block/attributes/Block.hpp"
#include "block/attributes/Blocks.hpp"
#include "client/render/TileColor.hpp"
#include "world/poi/Coordinate.hpp"
#include "world/poi/Position.hpp"

namespace chunk {
namespace adapter {
class ChunkDataPacketAdapter;
}

struct ChunkGenSettings {
  const static int CHUNK_HEIGHT = 319;
  const static int GROUND_LEVEL = 62;
  const static int CHUNK_WIDTH = 16;
  const static int CHUNK_BOTTOM_LIMIT = -64;
  const static int CHUNK_UPPER_LIMIT = CHUNK_HEIGHT - CHUNK_BOTTOM_LIMIT;
};  // chunk configurations

class Chunk {
 private:
  using ChunkPosT = coordinate::ChunkPositionT;
  using BlockPosT = coordinate::BlockPositionT;
  using PixelPosT = coordinate::PixelPositonT;

 private:
  class Iterator;

 public:
  Chunk() = delete;

  // for world generation - only need to store the block type
  explicit Chunk(ChunkPosT chunkPos,
                 block::Block* (*blocks)[ChunkGenSettings::CHUNK_WIDTH]
                                        [ChunkGenSettings::CHUNK_HEIGHT]) {
    this->chunkPos = chunkPos;
    copyArray(*blocks, chunkBlocks);
    initPosition();
    initLight();
    updateHeightMap();
  }

  // for loading from file - need to store the block type, direction and light
  // level
  explicit Chunk(
      ChunkPosT chunkPos,
      block::Block* (*blocks)[ChunkGenSettings::CHUNK_WIDTH]
                             [ChunkGenSettings::CHUNK_HEIGHT],
      Direction::DirectionT (*blockDirection)[ChunkGenSettings::CHUNK_WIDTH]
                                             [ChunkGenSettings::CHUNK_HEIGHT],
      TileColor::TileColorT (*tileLights)[ChunkGenSettings::CHUNK_WIDTH]
                                         [ChunkGenSettings::CHUNK_HEIGHT]) {
    this->chunkPos = chunkPos;
    copyArray(*blocks, chunkBlocks);
    initPosition();
    initDirection(blockDirection);
    initLight(tileLights);
    updateHeightMap();
  }

  ~Chunk() {
    for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
        delete getBlock(x_pos, y_pos);
      }
    }
  }

  static std::unique_ptr<Chunk> emptyChunk(ChunkPosT chunkPos) {
    block::Block* blocks[chunk::ChunkGenSettings::CHUNK_WIDTH]
                        [chunk::ChunkGenSettings::CHUNK_HEIGHT];
    for (auto x_pos = 0; x_pos < chunk::ChunkGenSettings::CHUNK_WIDTH;
         x_pos++) {
      for (auto z_pos = 0; z_pos < chunk::ChunkGenSettings::CHUNK_HEIGHT;
           z_pos++) {
        auto block = block::Blocks::getInstance().AIR_BLOCK->createObject();
        blocks[x_pos][z_pos] = block;
      }
    }
    return std::make_unique<Chunk>(chunkPos, &blocks);
  }

  [[nodiscard]] block::Block* getBlock(BlockPosT x, BlockPosT z) const {
    return chunkBlocks[x][z];
  }

  [[nodiscard]] block::Block* getBlock(
      const coordinate::BlockPos& blockPos) const {
    return chunkBlocks[blockPos.x][blockPos.z];
  }

  [[nodiscard]] uint32_t getRGBA(BlockPosT x, BlockPosT z) const {
    return blockLight[x][z];
  }

  [[nodiscard]] uint8_t getBlockLightLevel(BlockPosT x, BlockPosT z) const {
    return TileColor::convertToR(blockLight[x][z]) / 17;
  }

  [[nodiscard]] uint8_t getBlockLightLevel(
      const coordinate::BlockPos& blockPos) const {
    return TileColor::convertToR(blockLight[blockPos.x][blockPos.z]) / 17;
  }

  [[nodiscard]] block::Block* getBlockWithBoundaryCheck(
      const coordinate::ChunkPos& chunkBlockPos) const {
    if ((chunkBlockPos.blockPos.z >= 0 &&
         chunkBlockPos.blockPos.z < ChunkGenSettings::CHUNK_HEIGHT) &&
        (chunkBlockPos.blockPos.x >= 0 &&
         chunkBlockPos.blockPos.x < ChunkGenSettings::CHUNK_WIDTH))
      return getBlock(chunkBlockPos.blockPos);
    return block::Blocks::getInstance().getObjectInstance(
        "minecraft:error_block");
  }

  [[nodiscard]] bool isBlockExposedToDaylight(BlockPosT x, BlockPosT z) const {
    return heightMap[x] <= z;
  }

  [[nodiscard]] ChunkPosT getChunkPosition() const { return chunkPos; }

  // get the position of the highest non-air block
  BlockPosT getHeightMap(BlockPosT x) { return heightMap[x]; }

  /**
   * @brief set the light level of the block
   * @param x pos
   * @param z pos
   * @param light brightness, from 0-15
   */
  void setBlockLightLevel(BlockPosT x, BlockPosT z, uint8_t light) {
    auto brightness = light * 17;  // 255/15=17
    auto rgba =
        TileColor::convertToRGBA(brightness, brightness, brightness, 255);
    getBlock(x, z)->setTileColor(rgba);
    blockLight[x][z] = rgba;
  }

  void setBlockPosition(ChunkPosT x, ChunkPosT z, block::Block* block) {
    if (getBlock(x, z) != nullptr)
      delete getBlock(x, z);
    chunkBlocks[x][z] = block;
    auto block_position = toBlockPosition(chunkPos, coordinate::BlockPos(x, z));
    getBlock(x, z)->setPosition(block_position);
    updateHeightMap(x, z);
  }

  static coordinate::ChunkPos toChunkPosition(BlockPosT blockPosX,
                                              BlockPosT blockPosZ) {
    coordinate::ChunkPos chunk_settings;

    if (blockPosX == 0) [[unlikely]] {
      chunk_settings.chunkPos = 0;
    } else [[likely]] {
      if (blockPosX > 0) {
        chunk_settings.chunkPos = blockPosX / 16;
      } else {
        chunk_settings.chunkPos = ((blockPosX + 1) / 16 - 1);
      }
    }
    chunk_settings.blockPos.x =
        blockPosX - ChunkGenSettings::CHUNK_WIDTH * chunk_settings.chunkPos;
    chunk_settings.blockPos.z = blockPosZ;
    return chunk_settings;
  }

  static coordinate::BlockPos toBlockPosition(ChunkPosT chunkPos,
                                              BlockPosT x,
                                              BlockPosT z) {
    return {x + ChunkGenSettings::CHUNK_WIDTH * chunkPos, z};
  }

  static coordinate::BlockPos toBlockPosition(
      ChunkPosT chunkPos,
      const coordinate::BlockPos& chunkBlockPos) {
    return toBlockPosition(chunkPos, chunkBlockPos.x, chunkBlockPos.z);
  }

  static ChunkPosT convertToChunkPos(PixelPosT pixelPos) {
    auto zoom = RenderSystem::Settings::pixelProportion;
    return (int)(pixelPos / (double)(zoom * ChunkGenSettings::CHUNK_WIDTH));
  }

  void update() {}

  void render() const {
    for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
        RenderSystem::render(*getBlock(x_pos, y_pos));
      }
    }
  }

  Iterator begin() {
    return {chunkBlocks[0][0], ChunkGenSettings::CHUNK_WIDTH,
            ChunkGenSettings::CHUNK_HEIGHT};
  }

  static Iterator end() {
    return {nullptr, ChunkGenSettings::CHUNK_WIDTH,
            ChunkGenSettings::CHUNK_HEIGHT};
  }

 private:
  friend class adapter::ChunkDataPacketAdapter;

  // the position index of the chunk
  ChunkPosT chunkPos = -1;
  // store the pointer of each block in the chunk
  block::Block* chunkBlocks[ChunkGenSettings::CHUNK_WIDTH]
                           [ChunkGenSettings::CHUNK_HEIGHT]{};
  // the tile color(light) on each block
  TileColor::TileColorT blockLight[ChunkGenSettings::CHUNK_WIDTH]
                                  [ChunkGenSettings::CHUNK_HEIGHT]{};
  // record the light source in the chunk
  std::map<coordinate::BlockPos, uint8_t> lightSourceMap;
  // record the highest point in the chunk-> use to calculate the blocks
  // influenced by daylight
  std::array<BlockPosT, ChunkGenSettings::CHUNK_WIDTH> heightMap{};

  template <typename T>
  void copyArray(T src, T dest) {
    for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
        dest[x_pos][y_pos] = src[x_pos][y_pos];
      }
    }
  }

  // configure the block position
  void initPosition() const {
    for (auto x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (auto z_pos = 0; z_pos < ChunkGenSettings::CHUNK_HEIGHT; ++z_pos) {
        auto block_position =
            toBlockPosition(chunkPos, coordinate::BlockPos(x_pos, z_pos));
        getBlock(x_pos, z_pos)->setPosition(block_position);
      }
    }
  }

  // configure the block direction
  void initDirection(Direction::DirectionT (
      *blockDirection)[ChunkGenSettings::CHUNK_WIDTH]
                      [ChunkGenSettings::CHUNK_HEIGHT]) const {
    for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
        getBlock(x_pos, y_pos)
            ->setDirection(
                (Direction::DirectionType)(*blockDirection)[x_pos][y_pos]);
      }
    }
  }

  // load the block light level
  void initLight(uint32_t (
      *lights)[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT]) {
    copyArray(*lights, blockLight);
    for (auto x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (auto z_pos = 0; z_pos < ChunkGenSettings::CHUNK_HEIGHT; ++z_pos) {
        getBlock(x_pos, z_pos)->setTileColor((*lights)[x_pos][z_pos]);
      }
    }
  }

  void initLight() {
    for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
        setBlockLightLevel(x_pos, y_pos, 0);
      }
    }
  }

  /**
   * @brief update the height map
   * @param x
   * @param z
   */
  void updateHeightMap(BlockPosT x, BlockPosT z) {
    if (heightMap[x] < z)
      heightMap[x] = z;
  }

  // update the highest non-air block to the container
  void updateHeightMap() {
    for (auto x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
      for (auto z_pos = ChunkGenSettings::CHUNK_HEIGHT - 1; z_pos >= 0;
           --z_pos) {
        if (!getBlock(x_pos, z_pos)->isAir()) {
          updateHeightMap(x_pos, z_pos);
          continue;
        }
      }
    }
  }

  class Iterator {
   private:
    block::Block* current;
    block::Block* start;
    int rows;
    int cols;

   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = block::Block;
    using difference_type = std::ptrdiff_t;
    using pointer = block::Block*;
    using reference = block::Block&;

    Iterator(block::Block* start, int rows, int cols)
        : current(start), start(start), rows(rows), cols(cols) {}

    bool operator==(const Iterator& other) const {
      return current == other.current;
    }

    bool operator!=(const Iterator& other) const { return !(*this == other); }

    reference operator*() const { return *current; }

    pointer operator->() const { return current; }

    Iterator& operator++() {
      current++;
      if (reinterpret_cast<char*>(current) - reinterpret_cast<char*>(start) >=
          rows * cols * sizeof(block::Block)) {
        current = nullptr;
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }
  };
};
}  // namespace chunk

#endif  // MINECRAFT_CHUNK_HPP
