//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_DEEPSLATEBLOCK_HPP
#define MINECRAFT_DEEPSLATEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
	class DeepslateBlock : public block::Block {
	private:
	public:
		DeepslateBlock() : Block("deepslate_block") {
		}

		Block* createObject() override {
			return new DeepslateBlock;
		}
	};
}

#endif //MINECRAFT_DEEPSLATEBLOCK_HPP
