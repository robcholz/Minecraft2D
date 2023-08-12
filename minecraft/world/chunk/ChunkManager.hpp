//
// Created by robcholz on 8/12/23.
//

#ifndef MINECRAFT_2D_CHUNKMANAGER_HPP
#define MINECRAFT_2D_CHUNKMANAGER_HPP


#include "ChunkStream.hpp"
#include "ChunkManagerAccess.hpp"

namespace chunk {
	class ChunkManager : public ChunkStream,
	                     public ChunkManagerAccess {
	private:
		using ChunkPosT = coordinate::ChunkPositionT;
		using BlockPosition = coordinate::BlockPosition;
	public:
		ChunkManager() = delete;

		explicit ChunkManager(WorldAccess* worldAccess, ChunkStream::DistanceT simulationDistance, ChunkStream::DistanceT renderDistance) :
				ChunkStream(worldAccess, simulationDistance, renderDistance) {
		}

		~ChunkManager() override = default;

		Chunk* getChunk(ChunkPosT chunkPos) override {
			return ChunkStream::getChunk(chunkPos);
		}

		ChunkStreamAccess* getChunkStream() override {
			return this;
		}

		block::Block* getBlock(const BlockPosition& blockCoordinate) override {
			return getBlock(blockCoordinate.get());
		}

		block::Block* getBlock(const coordinate::BlockPos& blockCoordinate) override {
			// convert to chunk position and search in stream
			// locate the block pointer in a specific chunk
			auto chunkSettings = Chunk::toChunkSettings(blockCoordinate.x, blockCoordinate.z);
			auto chunk = this->getChunk(chunkSettings.chunkPos);
			if (chunk != nullptr)
				return chunk->getBlockWithBoundaryCheck(chunkSettings);
			PLOG_ERROR << "Given block doesn't exist in the loaded chunks: ChunkPosition: " << chunkSettings.chunkPos;
			return block::Blocks::getInstance().getObjectInstance("minecraft:error_block");
		}
	};
}

#endif //MINECRAFT_2D_CHUNKMANAGER_HPP
