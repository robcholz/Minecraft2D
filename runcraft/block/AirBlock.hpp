//
// Created by robcholz on 4/16/23.
//

#ifndef RUNCRAFT_AIRBLOCK_HPP
#define RUNCRAFT_AIRBLOCK_HPP

#include "Block.hpp"

namespace block::blocks{
		class AirBlock : public block::Block {
		public:
			AirBlock() : Block("air_block") {

			}
		};
	}

#endif //RUNCRAFT_AIRBLOCK_HPP
