//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_BLOCKS_HPP
#define MINECRAFT_BLOCKS_HPP

#include <memory>
#include "block/AirBlock.hpp"
#include "block/AndesiteBlock.hpp"
#include "block/BedrockBlock.hpp"
#include "block/BirchLogBlock.hpp"
#include "block/CoalOreBlock.hpp"
#include "block/CobblestoneBlock.hpp"
#include "block/DeepslateBlock.hpp"
#include "block/DiamondBlock.hpp"
#include "block/DioriteBlock.hpp"
#include "block/DirtBlock.hpp"
#include "block/ErrorBlock.hpp"
#include "block/GlowstoneBlock.hpp"
#include "block/GraniteBlock.hpp"
#include "block/GrassBlock.hpp"
#include "block/IronBlock.hpp"
#include "block/StoneBlock.hpp"
#include "block/attributes/Block.hpp"
#include "block/attributes/BlockSoundGroup.hpp"
#include "util/Registry.hpp"

#define DeclareBlock(block) BlockPtr block = nullptr

namespace block {
class Blocks : public utils::Registry<Block> {
 private:
  using String = std::string;
  using BlockPtr = Block::BlockPtr;

 public:
  DeclareBlock(AIR_BLOCK);
  DeclareBlock(BEDROCK_BLOCK);
  DeclareBlock(BIRCH_LOG_BLOCK);
  DeclareBlock(COAL_ORE_BLOCK);
  DeclareBlock(COBBLESTONE_BLOCK);
  DeclareBlock(DIAMOND_BLOCK);
  DeclareBlock(DIRT_BLOCK);
  DeclareBlock(ERROR_BLOCK);
  DeclareBlock(GRASS_BLOCK);
  DeclareBlock(IRON_BLOCK);
  DeclareBlock(STONE_BLOCK);
  DeclareBlock(DEEPSLATE_BLOCK);
  DeclareBlock(ANDESITE_BLOCK);
  DeclareBlock(DIORITE_BLOCK);
  DeclareBlock(GRANITE_BLOCK);
  DeclareBlock(GLOWSTONE_BLOCK);

  static Blocks& getInstance() {
    static Blocks instance;
    return instance;
  }

 private:
  Blocks() { onRegister(); }
  ~Blocks() override = default;

  void onRegister() {
    regBlock();
    regBlockSound();
  }

  void regBlock() {
    AIR_BLOCK = registerObject<AirBlock>();
    BEDROCK_BLOCK = registerObject<BedrockBlock>();
    BIRCH_LOG_BLOCK = registerObject<BirchLogBlock>();
    COAL_ORE_BLOCK = registerObject<CoalOreBlock>();
    COBBLESTONE_BLOCK = registerObject<CobblestoneBlock>();
    DIAMOND_BLOCK = registerObject<DiamondBlock>();
    DIRT_BLOCK = registerObject<DirtBlock>();
    ERROR_BLOCK = registerObject<ErrorBlock>();
    GRASS_BLOCK = registerObject<GrassBlock>();
    IRON_BLOCK = registerObject<IronBlock>();
    STONE_BLOCK = registerObject<StoneBlock>();
    DEEPSLATE_BLOCK = registerObject<DeepslateBlock>();
    ANDESITE_BLOCK = registerObject<AndesiteBlock>();
    DIORITE_BLOCK = registerObject<DioriteBlock>();
    GRANITE_BLOCK = registerObject<GraniteBlock>();
    GLOWSTONE_BLOCK = registerObject<GlowstoneBlock>();
  }

  void regBlockSound() const {
    BlockSoundGroup::bind(STONE_BLOCK,
                          SoundEvents::getInstance().STEP_SOUND_STONE);
    BlockSoundGroup::bind(GRASS_BLOCK,
                          SoundEvents::getInstance().STEP_SOUND_GRASS);
    BlockSoundGroup::bind(BEDROCK_BLOCK,
                          SoundEvents::getInstance().STEP_SOUND_GRASS);
  }
};
}  // namespace block

#endif  // MINECRAFT_BLOCKS_HPP
