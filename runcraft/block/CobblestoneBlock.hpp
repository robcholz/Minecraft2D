//
// Created by robcholz on 6/9/23.
//

#ifndef RUNCRAFT_COBBLESTONEBLOCK_HPP
#define RUNCRAFT_COBBLESTONEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class CobblestoneBlock : public block::Block {
	private:
	public:
		CobblestoneBlock() : Block("cobblestone_block") {
		}

		Block* getBlockInstance() override {
			return new CobblestoneBlock;
		}
	};
}

#endif //RUNCRAFT_BIRCHLOGBLOCK_HPP
