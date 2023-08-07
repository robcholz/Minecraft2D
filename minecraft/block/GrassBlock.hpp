//
// Created by robcholz on 4/13/23.
//

#ifndef MINECRAFT_GRASSBLOCK_HPP
#define MINECRAFT_GRASSBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class GrassBlock : public block::Block {
	private:
	public:
		explicit GrassBlock() : Block("grass_block") {
		}

		Block* newBlock() override {
			return new GrassBlock;
		}
	};
}


#endif //MINECRAFT_GRASSBLOCK_HPP
