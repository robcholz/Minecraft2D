//
// Created by robcholz on 7/2/23.
//

#ifndef RUNCRAFT_DEEPSLATEBLOCK_HPP
#define RUNCRAFT_DEEPSLATEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class DeepslateBlock : public block::Block {
	private:
	public:
		DeepslateBlock() : Block("deepslate_block") {
		}

		Block* newBlock() override {
			return new DeepslateBlock;
		}
	};
}

#endif //RUNCRAFT_DEEPSLATEBLOCK_HPP
