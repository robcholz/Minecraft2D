
//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_DIORITEBLOCK_HPP
#define MINECRAFT_DIORITEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class DioriteBlock : public block::Block {
	private:
	public:
		DioriteBlock() : Block("diorite_block") {
		}

		Block* newBlock() override {
			return new DioriteBlock;
		}
	};
}

#endif //MINECRAFT_DIORITEBLOCK_HPP
