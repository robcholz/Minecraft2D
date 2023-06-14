//
// Created by robcholz on 6/9/23.
//

#ifndef RUNCRAFT_COALOREBLOCK_HPP
#define RUNCRAFT_COALOREBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class CoalOreBlock : public block::Block {
	private:
	public:
		CoalOreBlock() : Block("coal_ore_block") {
		}

		Block* newBlock() override {
			return new CoalOreBlock;
		}
	};
}

#endif //RUNCRAFT_BIRCHLOGBLOCK_HPP
