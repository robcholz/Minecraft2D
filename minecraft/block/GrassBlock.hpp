//
// Created by robcholz on 4/13/23.
//

#ifndef MINECRAFT_GRASSBLOCK_HPP
#define MINECRAFT_GRASSBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
class GrassBlock : public block::Block {
 private:
 public:
  explicit GrassBlock() : Block("grass_block") {}

  Block* createObject() override { return new GrassBlock; }
};
}  // namespace block

#endif  // MINECRAFT_GRASSBLOCK_HPP
