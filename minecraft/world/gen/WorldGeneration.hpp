//
// Created by robcholz on 5/3/23.
//

#ifndef MINECRAFT_WORLDGENERATION_HPP
#define MINECRAFT_WORLDGENERATION_HPP

#include "util/math/noise/PerlinNoise.hpp"
#include "world/chunk/Chunk.hpp"
#include "world/gen/carver/Carver.hpp"
#include "world/gen/decorator/HeightDecorator.hpp"
#include "world/poi/Coordinate.hpp"

class WorldGeneration {
 private:
  using BlockPosT = coordinate::BlockPositionT;
  using ChunkPosT = coordinate::ChunkPositionT;

 protected:
  void generateBlock(ChunkPosT chunkPos) {
    for (auto x_pos = 0; x_pos < chunk::ChunkGenSettings::CHUNK_WIDTH;
         x_pos++) {
      auto x_world = (chunk::ChunkGenSettings::CHUNK_WIDTH * chunkPos + x_pos);
      for (auto z_pos = 0; z_pos < chunk::ChunkGenSettings::CHUNK_HEIGHT;
           z_pos++) {
        heightDecorator->decorate(chunkBlocksBuffer, chunkPos, x_pos, z_pos,
                                  x_world);
      }
    }
    /*
            for (unsigned int z = 0; z < chunk::ChunkGenSettings::CHUNK_HEIGHT;
       ++z) { carver->curve(chunkBlocksBuffer, x_world, x_pos, z, 150,
                                  [](BlockPosT z) { return z <=
       DEEPSLATE_BASE_HEIGHT; }); carver->curve(chunkBlocksBuffer, x_world,
       x_pos, z, 200,
                                              [](BlockPosT z) { return z >
       DEEPSLATE_BASE_HEIGHT; });
            }
            */
  }

 public:
  explicit WorldGeneration(unsigned int seed)
      : surfaceLayer{block::Blocks::getInstance().GRASS_BLOCK, 1,
                     HeightDecorator::LayerPriority::SURFACE},
        dirtLayer{block::Blocks::getInstance().DIRT_BLOCK, 4,
                  HeightDecorator::LayerPriority::TOP},
        stoneLayer{block::Blocks::getInstance().STONE_BLOCK, 50,
                   HeightDecorator::LayerPriority::TRANSITION},
        deepslateLayer{block::Blocks::getInstance().DEEPSLATE_BLOCK, 50,
                       HeightDecorator::LayerPriority::MIDDLE},
        bedrockLayer{block::Blocks::getInstance().BEDROCK_BLOCK, 5,
                     HeightDecorator::LayerPriority::BOTTOM} {
    carver = std::make_unique<Carver>(seed);
    heightDecorator = std::make_unique<HeightDecorator>(seed);

    heightDecorator->configure(surfaceLayer)
        .configure(dirtLayer)
        .configure(stoneLayer)
        .configure(deepslateLayer)
        .configure(bedrockLayer)
        .setLayerBlendModifier(dirtLayer, stoneLayer, 3)
        .setLayerBlendModifier(stoneLayer, deepslateLayer, 3)
        .setLayerBlendModifier(deepslateLayer, bedrockLayer, 4);
  }

  chunk::Chunk* getChunk(ChunkPosT chunkPos) {
    block::Block* blocks[chunk::ChunkGenSettings::CHUNK_WIDTH]
                        [chunk::ChunkGenSettings::CHUNK_HEIGHT];
    generateBlock(chunkPos);
    for (auto x_pos = 0; x_pos < chunk::ChunkGenSettings::CHUNK_WIDTH;
         x_pos++) {
      for (auto z_pos = 0; z_pos < chunk::ChunkGenSettings::CHUNK_HEIGHT;
           z_pos++) {
        auto block_serial_id = chunkBlocksBuffer[x_pos][z_pos];
        auto block = block::Blocks::getInstance().createObject(block_serial_id);
        blocks[x_pos][z_pos] = block;
      }
    }
    auto chunk = new chunk::Chunk(chunkPos, &blocks);
    return chunk;
  }

 private:
  HeightDecorator::LayerConfig surfaceLayer;
  HeightDecorator::LayerConfig dirtLayer;
  HeightDecorator::LayerConfig stoneLayer;
  HeightDecorator::LayerConfig deepslateLayer;
  HeightDecorator::LayerConfig bedrockLayer;

  std::unique_ptr<Carver> carver;
  std::unique_ptr<HeightDecorator> heightDecorator;

  block::ID::SerialIDT
      chunkBlocksBuffer[chunk::ChunkGenSettings::CHUNK_WIDTH]
                       [chunk::ChunkGenSettings::CHUNK_HEIGHT]{};
};

#endif  // MINECRAFT_WORLDGENERATION_HPP
