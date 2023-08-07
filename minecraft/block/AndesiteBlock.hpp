//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_ANDESITEBLOCK_HPP
#define MINECRAFT_ANDESITEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class AndesiteBlock : public block::Block {
	private:
	public:
		AndesiteBlock() : Block("andesite_block") {
		}

		Block* newBlock() override {
			return new AndesiteBlock;
		}
	};
}

#endif //MINECRAFT_ANDESITEBLOCK_HPP
