//
// Created by robcholz on 8/12/23.
//

#ifndef MINECRAFT_2D_WORLDCHUNK_HPP
#define MINECRAFT_2D_WORLDCHUNK_HPP


#include "ChunkStream.hpp"
#include "WorldChunkAccess.hpp"
#include "world/chunk/light/LightingProvider.hpp"

namespace chunk {
	class WorldChunk : public ChunkStream,
	                   public WorldChunkAccess {
	private:
		using ChunkPosT = coordinate::ChunkPositionT;
		using BlockPosT = coordinate::BlockPositionT;
		using BlockPosition = coordinate::BlockPosition;
	public:
		WorldChunk() = delete;

		explicit WorldChunk(WorldAccess* worldAccess, ChunkStream::DistanceT simulationDistance, ChunkStream::DistanceT renderDistance) :
				ChunkStream(worldAccess, simulationDistance, renderDistance) {
			lightingProvider = std::make_unique<LightingProvider>(this);
			ChunkStream::onChunkRender([this](ChunkPosT chunkPos) { lightingProvider->updateDaylight(chunkPos); });
		}

		~WorldChunk() override = default;

		Chunk* getChunk(ChunkPosT chunkPos) override {
			return ChunkStream::getChunk(chunkPos);
		}

		ChunkStreamAccess* getChunkStream() override {
			return this;
		}

		uint8_t getBlockLightLevel(const coordinate::BlockPos& blockPosition) override {
			return getBlockLightLevel(blockPosition.x, blockPosition.z);
		}

		uint8_t getBlockLightLevel(BlockPosT x, BlockPosT z) override {
			auto chunkSettings = Chunk::toChunkPosition(x, z);
			auto chunk = this->getChunk(chunkSettings.chunkPos);
			return chunk->getBlockLightLevel(chunkSettings.blockPos);
		}

		block::Block* getBlock(const BlockPosition& blockPosition) override {
			return getBlock(blockPosition.get());
		}

		block::Block* getBlock(const coordinate::BlockPos& blockPosition) override {
			// convert to chunk position and search in stream
			// locate the block pointer in a specific chunk
			auto chunkSettings = Chunk::toChunkPosition(blockPosition.x, blockPosition.z);
			auto chunk = this->getChunk(chunkSettings.chunkPos);
			if (chunk != nullptr)
				return chunk->getBlockWithBoundaryCheck(chunkSettings);
			PLOG_ERROR << "Given block doesn't exist in the loaded chunks: ChunkPosition: " << chunkSettings.chunkPos;
			return block::Blocks::getInstance().getObjectInstance("minecraft:error_block");
		}

		void setBlockLightLevel(BlockPosT x, BlockPosT z, uint8_t lightLevel) override {
			auto chunkSettings = Chunk::toChunkPosition(x, z);
			auto chunk = this->getChunk(chunkSettings.chunkPos);
			chunk->setBlockLightLevel(chunkSettings.blockPos.x, chunkSettings.blockPos.z, lightLevel);
		}

		void setBlockLightLevel(const coordinate::BlockPos& blockPosition, uint8_t lightLevel) override {
			setBlockLightLevel(blockPosition.x, blockPosition.z, lightLevel);
		}

		// main chunk update function
		void onUpdate() override {
			lightingProvider->update();
		}

	private:
		std::unique_ptr<LightingProvider> lightingProvider;
	};
}

#endif //MINECRAFT_2D_WORLDCHUNK_HPP
