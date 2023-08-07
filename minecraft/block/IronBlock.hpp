//
// Created by robcholz on 6/5/23.
//

#ifndef MINECRAFT_IRONBLOCK_HPP
#define MINECRAFT_IRONBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks{

	class IronBlock: public block::Block{
	public:
		IronBlock(): Block("iron_block"){

		}

		Block* newBlock() override {
			return new IronBlock;
		}
	};
}

#endif //MINECRAFT_IRONBLOCK_HPP
