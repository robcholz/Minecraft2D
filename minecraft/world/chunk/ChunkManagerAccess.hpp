//
// Created by robcholz on 8/12/23.
//

#ifndef MINECRAFT_2D_CHUNKMANAGERACCESS_HPP
#define MINECRAFT_2D_CHUNKMANAGERACCESS_HPP


#include "block/attributes/Block.hpp"
#include "ChunkStreamAccess.hpp"


namespace chunk {
	class ChunkManagerAccess {
	private:
		using ChunkPosT = coordinate::ChunkPositionT;
	public:
		virtual Chunk* getChunk(ChunkPosT chunkPos) = 0;

		virtual ChunkStreamAccess* getChunkStream() = 0;

		virtual block::Block* getBlock(const coordinate::BlockPosition& blockPosition) = 0;

		virtual block::Block* getBlock(const coordinate::BlockPos& blockPosition) = 0;
	};
}

#endif //MINECRAFT_2D_CHUNKMANAGERACCESS_HPP
