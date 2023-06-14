//
// Created by robcholz on 6/11/23.
//

#ifndef RUNCRAFT_CHUNKSTREAMACCESS_HPP
#define RUNCRAFT_CHUNKSTREAMACCESS_HPP

#include "block/attributes/Block.hpp"

namespace chunk {
	class ChunkStreamAccess {
	public:
		virtual block::Block* getBlock(coordinate::Coordinate<coordinate::BlockPositionT> blockCoordinate) = 0;
	};
}

#endif //RUNCRAFT_CHUNKSTREAMACCESS_HPP
