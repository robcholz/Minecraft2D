//
// Created by robcholz on 6/11/23.
//

#ifndef MINECRAFT_CHUNKSTREAMACCESS_HPP
#define MINECRAFT_CHUNKSTREAMACCESS_HPP

#include "block/attributes/Block.hpp"

namespace chunk {
	class ChunkStreamAccess {
	public:
		virtual block::Block* getBlock(const coordinate::BlockPosition& blockPosition) = 0;
		virtual block::Block* getBlock(const coordinate::BlockPos& blockPosition) = 0;

		virtual int getRenderedChunks() = 0;

		virtual int getUpdatedChunks() = 0;

		virtual int getDeletedChunks() = 0;

		virtual int getCachedChunks() = 0;
	};
}

#endif //MINECRAFT_CHUNKSTREAMACCESS_HPP
