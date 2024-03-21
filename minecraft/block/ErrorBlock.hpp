//
// Created by robcholz on 6/12/23.
//

#ifndef MINECRAFT_ERRORBLOCK_HPP
#define MINECRAFT_ERRORBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
class ErrorBlock : public block::Block {
 private:
 public:
  ErrorBlock() : Block("error_block") { enableHitbox(false); }

  Block* createObject() override { return new ErrorBlock; }
};
}  // namespace block

#endif  // MINECRAFT_ERRORBLOCK_HPP
