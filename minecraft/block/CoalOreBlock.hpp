//
// Created by robcholz on 6/9/23.
//

#ifndef MINECRAFT_COALOREBLOCK_HPP
#define MINECRAFT_COALOREBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
	class CoalOreBlock : public block::Block {
	private:
	public:
		CoalOreBlock() : Block("coal_ore_block") {
		}

		Block* createObject() override {
			return new CoalOreBlock;
		}
	};
}

#endif //MINECRAFT_COALOREBLOCK_HPP
