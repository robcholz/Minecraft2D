//
// Created by robcholz on 7/2/23.
//

#ifndef RUNCRAFT_ANDESITEBLOCK_HPP
#define RUNCRAFT_ANDESITEBLOCK_HPP

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

#endif //RUNCRAFT_ANDESITEBLOCK_HPP
