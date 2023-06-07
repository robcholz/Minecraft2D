//
// Created by robcholz on 6/5/23.
//

#ifndef RUNCRAFT_IRONBLOCK_HPP
#define RUNCRAFT_IRONBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks{

	class IronBlock: public block::Block{
	public:
		IronBlock(): Block("iron_block"){

		}

		Block* getBlockInstance() override {
			return new IronBlock;
		}
	};
}

#endif //RUNCRAFT_IRONBLOCK_HPP
