//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_GRANITEBLOCK_HPP
#define MINECRAFT_GRANITEBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
	class GraniteBlock : public block::Block {
	private:
	public:
		GraniteBlock() : Block("granite_block") {
		}

		Block* createObject() override {
			return new GraniteBlock;
		}
	};
}

#endif //MINECRAFT_GRANITEBLOCK_HPP
