//
// Created by robcholz on 4/16/23.
//

#ifndef RUNCRAFT_DIRTBLOCK_HPP
#define RUNCRAFT_DIRTBLOCK_HPP

#include "Block.hpp"


namespace block::blocks{

	class DirtBlock: public block::Block{
	public:
		DirtBlock(): Block("dirt_block"){

		}
	};
}

#endif //RUNCRAFT_DIRTBLOCK_HPP
