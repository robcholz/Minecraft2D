//
// Created by robcholz on 6/9/23.
//

#ifndef RUNCRAFT_DIAMONDBLOCK_HPP
#define RUNCRAFT_DIAMONDBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class DiamondBlock : public block::Block {
	private:
	public:
		DiamondBlock() : Block("diamond_block") {
		}

		Block* newBlock() override {
			return new DiamondBlock;
		}
	};
}

#endif //RUNCRAFT_DIAMONDBLOCK_HPP
