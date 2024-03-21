//
// Created by robcholz on 7/1/23.
//

#ifndef MINECRAFT_STONEBLOCK_HPP
#define MINECRAFT_STONEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {

class StoneBlock : public block::Block {
 public:
  StoneBlock() : Block("stone_block") {}

  Block* createObject() override { return new StoneBlock; }
};
}  // namespace block

#endif  // MINECRAFT_STONEBLOCK_HPP
