//
// Created by robcholz on 7/2/23.
//

#ifndef RUNCRAFT_GRANITEBLOCK_HPP
#define RUNCRAFT_GRANITEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class GraniteBlock : public block::Block {
	private:
	public:
		GraniteBlock() : Block("granite_block") {
		}

		Block* newBlock() override {
			return new GraniteBlock;
		}
	};
}

#endif //RUNCRAFT_GRANITEBLOCK_HPP
