//
// Created by robcholz on 4/16/23.
//

#ifndef RUNCRAFT_DIRTBLOCK_HPP
#define RUNCRAFT_DIRTBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class DirtBlock : public block::Block {
	private:
	public:
		DirtBlock() : Block("dirt_block") {
		}

		Block* getBlockInstance() override {
			return new DirtBlock;
		}
	};
}

#endif //RUNCRAFT_DIRTBLOCK_HPP
