//
// Created by robcholz on 4/16/23.
//

#ifndef RUNCRAFT_AIRBLOCK_HPP
#define RUNCRAFT_AIRBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class AirBlock : public block::Block {
	private:
	public:
		explicit AirBlock() : Block("air_block") {
		}

		Block* getBlockInstance() override {
			return new AirBlock;
		}
	};
}

#endif //RUNCRAFT_AIRBLOCK_HPP
