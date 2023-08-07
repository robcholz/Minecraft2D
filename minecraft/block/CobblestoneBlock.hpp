//
// Created by robcholz on 6/9/23.
//

#ifndef MINECRAFT_COBBLESTONEBLOCK_HPP
#define MINECRAFT_COBBLESTONEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class CobblestoneBlock : public block::Block {
	private:
	public:
		CobblestoneBlock() : Block("cobblestone_block") {
		}

		Block* newBlock() override {
			return new CobblestoneBlock;
		}
	};
}

#endif //MINECRAFT_COBBLESTONEBLOCK_HPP
