//
// Created by robcholz on 4/16/23.
//

#ifndef MINECRAFT_DIRTBLOCK_HPP
#define MINECRAFT_DIRTBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
	class DirtBlock : public block::Block {
	private:
	public:
		DirtBlock() : Block("dirt_block") {
		}

		Block* createObject() override {
			return new DirtBlock;
		}
	};
}

#endif //MINECRAFT_DIRTBLOCK_HPP
