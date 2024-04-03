//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_HEIGHTDECORATOR_HPP
#define MINECRAFT_HEIGHTDECORATOR_HPP

#include <optional>
#include <unordered_map>
#include <utility>
#include "Decorator.hpp"

class HeightDecorator : public Decorator {
 private:
 protected:
 public:
  enum class LayerPriority : uint8_t {
    SURFACE = 0,
    TOP,
    TRANSITION,
    MIDDLE,
    BOTTOM,

    // DO NOT USE THIS ENUM
    INDEX_COUNT
  };

  struct LayerConfig {
    block::Block::BlockPtr block;
    BlockPosT thickness;
    LayerPriority layerPriority;
  };

  explicit HeightDecorator(unsigned int seed) {
    noise.initialize(seed);
    randomEngineForLayerBlender.seed(seed);
  }

  HeightDecorator& configure(const LayerConfig& layerConfig) {
    layers.insert({layerConfig.layerPriority, layerConfig});
    if (layerConfig.layerPriority == LayerPriority::BOTTOM) {
      for (const auto& ele_i : layers) {
        BlockPosT lower = 0, upper = 0;
        for (const auto& ele_k : layers)
          if (ele_k.second.layerPriority > ele_i.second.layerPriority)
            lower += ele_k.second.thickness;
        upper = lower + ele_i.second.thickness;
        layerInfo.insert({ele_i.second.layerPriority, {lower, upper}});
      }
    }
    return *this;
  }

  std::optional<BlockPosT> getLayerHeight(const block::Block& block) {}

  HeightDecorator& setLayerBlendModifier(const LayerConfig& config1,
                                         const LayerConfig& config2,
                                         uint8_t halfRange) {
    uint8_t stdev = halfRange;
    uint16_t key = static_cast<uint8_t>(config1.layerPriority) << 8 |
                   static_cast<uint8_t>(config2.layerPriority);
    uint16_t value = static_cast<uint8_t>(DEFAULT_DISTRI_MEAN) << 8 |
                     static_cast<uint8_t>(stdev);
    blendModifiers.insert({key, value});
    return *this;
  }

  void decorate(SerialT blockBuffer[chunk::ChunkGenSettings::CHUNK_WIDTH]
                                   [chunk::ChunkGenSettings::CHUNK_HEIGHT],
                BlockPosT chunkBlockPos,
                BlockPosT blockPosX,
                BlockPosT blockPosZ,
                BlockPosT worldPosX) override {
    blockBuffer[blockPosX][blockPosZ] =
        block::Blocks::getInstance().AIR_BLOCK->getSerialID();
    BlockPosT height_offset_l, height_offset_u;
    for (::size_t i = 0;
         static_cast<LayerPriority>(i) < LayerPriority::INDEX_COUNT; ++i) {
      auto priority = static_cast<LayerPriority>(i);
      if (priority == LayerPriority::SURFACE ||
          priority == LayerPriority::TOP ||
          priority == LayerPriority::TRANSITION) {
        height_offset_l = math::Math::floor(
            30 * noise.noise((float)worldPosX / 25.f, 0.9, 0.8) +
            30 * noise.noise((float)worldPosX / 50.f, 0.9, 0.8));
        height_offset_u = height_offset_l;
        if (priority == LayerPriority::TRANSITION)
          height_offset_l = 0;
      } else {
        height_offset_l = 0;
        height_offset_u = 0;
      }
      if (height_offset_l + layerInfo[priority].first <= blockPosZ &&
          blockPosZ < height_offset_u + layerInfo[priority].second)
        blockBuffer[blockPosX][blockPosZ] =
            layers[priority].block->getSerialID();
    }
  }

  bool blendLayerShouldGenerateLowerLayerBlock(BlockPosT worldBlockPosX,
                                               BlockPosT worldBlockPosZ,
                                               uint8_t halfRange) {
    //normalDistributionForLayerBlender.param(0,halfRange);
    // TODO
  }

 private:
  math::PerlinNoise noise;

  std::default_random_engine randomEngineForLayerBlender;
  //std::normal_distribution<BlockPosT> normalDistributionForLayerBlender;
  static constexpr uint8_t DEFAULT_DISTRI_MEAN = 0;

  std::unordered_map<LayerPriority, LayerConfig> layers;
  std::unordered_map<LayerPriority, std::pair<BlockPosT, BlockPosT>>
      layerInfo;  // lower, upper
  // value:: 8:mean, 8:stdev
  std::unordered_map<uint16_t, uint16_t> blendModifiers;
};

#endif  // MINECRAFT_HEIGHTDECORATOR_HPP
