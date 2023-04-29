//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCKS_HPP
#define RUNCRAFT_BLOCKS_HPP

#include "GrassBlock.hpp"
#include "AirBlock.hpp"
#include "DirtBlock.hpp"

namespace block::blocks{
	const static AirBlock AIR_BLOCK{};
	const static GrassBlock GRASS_BLOCK{};
	const static DirtBlock DIRT_BLOCK{};

	class Blocks{
	public:
		void render(){

		}
	};
}

#endif //RUNCRAFT_BLOCKS_HPP
