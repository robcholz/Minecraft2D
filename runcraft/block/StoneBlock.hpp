//
// Created by robcholz on 7/1/23.
//

#ifndef RUNCRAFT_STONEBLOCK_HPP
#define RUNCRAFT_STONEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks{

	class StoneBlock: public block::Block{
	public:
		StoneBlock(): Block("stone_block"){

		}

		Block* newBlock() override {
			return new StoneBlock;
		}
	};
}

#endif //RUNCRAFT_STONEBLOCK_HPP
