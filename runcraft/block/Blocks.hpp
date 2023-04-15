//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCKS_HPP
#define RUNCRAFT_BLOCKS_HPP

#include "Block.hpp"

class Blocks{
public:
	Block testBlock{"grass_block"};
	void render(){
		testBlock.render();
	}
};

#endif //RUNCRAFT_BLOCKS_HPP
