//
// Created by robcholz on 6/9/23.
//

#ifndef MINECRAFT_DIAMONDBLOCK_HPP
#define MINECRAFT_DIAMONDBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
class DiamondBlock : public block::Block {
 private:
 public:
  DiamondBlock() : Block("diamond_block") {}

  Block* createObject() override { return new DiamondBlock; }
};
}  // namespace block

#endif  // MINECRAFT_DIAMONDBLOCK_HPP
