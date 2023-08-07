//
// Created by robcholz on 6/9/23.
//

#ifndef MINECRAFT_BIRCHLOGBLOCK_HPP
#define MINECRAFT_BIRCHLOGBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class BirchLogBlock : public block::Block {
	private:
	public:
		BirchLogBlock() : Block("birch_log_block") {
		}

		Block* newBlock() override {
			return new BirchLogBlock;
		}
	};
}

#endif //MINECRAFT_BIRCHLOGBLOCK_HPP
