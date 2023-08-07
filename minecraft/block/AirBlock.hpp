//
// Created by robcholz on 4/16/23.
//

#ifndef MINECRAFT_AIRBLOCK_HPP
#define MINECRAFT_AIRBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
class AirBlock : public block::Block {
	private:
	public:
		explicit AirBlock() : Block("air_block") {
			enableHitbox(false);
		}

		Block* newBlock() override {
			return new AirBlock;
		}
	};
}

#endif //MINECRAFT_AIRBLOCK_HPP
