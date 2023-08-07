//
// Created by robcholz on 4/4/23.
//

#ifndef MINECRAFT_CHUNK_HPP
#define MINECRAFT_CHUNK_HPP

#include "bitsery/brief_syntax.h"
#include "world/poi/Coordinate.hpp"
#include "block/attributes/Block.hpp"
#include "world/poi/Position.hpp"
#include "block/attributes/Blocks.hpp"


namespace chunk {
	namespace adapter { class ChunkDataPacketAdapter; }

	struct ChunkGenSettings {
		const static int CHUNK_HEIGHT = 319;
		const static int GROUND_LEVEL = 62;
		const static int CHUNK_WIDTH = 16;
		const static int CHUNK_BOTTOM_LIMIT = -64;
		const static int CHUNK_UPPER_LIMIT = CHUNK_HEIGHT - CHUNK_BOTTOM_LIMIT;
	};// chunk configurations

	class Chunk {
	private:
		using ChunkPosT = coordinate::ChunkPositionT;
		using BlockPosT = coordinate::BlockPositionT;
		using PixelPosT = coordinate::PixelPositonT;
	public:
		explicit Chunk(ChunkPosT chunkPos) {
			this->chunkPos = chunkPos;
		}

		~Chunk() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					delete getBlock(x_pos, y_pos);
				}
			}
		}

		[[nodiscard]]
		block::Block* getBlock(BlockPosT x, BlockPosT z) const {
			return chunkBlocks[x][z];
		}

		[[nodiscard]]
		block::Block* getBlock(const coordinate::BlockPos& blockPos) const {
			return chunkBlocks[blockPos.x][blockPos.z];
		}

		[[nodiscard]]
		block::Block* getBlockWithBoundaryCheck(const coordinate::ChunkPos& chunkBlockPos) const {
			if ((chunkBlockPos.blockPos.z >= 0 && chunkBlockPos.blockPos.z < ChunkGenSettings::CHUNK_HEIGHT) &&
			    (chunkBlockPos.blockPos.x >= 0 && chunkBlockPos.blockPos.x < ChunkGenSettings::CHUNK_WIDTH))
				return getBlock(chunkBlockPos.blockPos);
			return block::blocks::Blocks::getInstance()->getBlockInstance("error_block");
		}

		void setBlockPosition(ChunkPosT x, ChunkPosT z, block::Block* block) {
			if (getBlock(x, z) != nullptr)
				delete getBlock(x, z);
			setBlock(x, z, block);
			auto block_position = toBlockPosition(chunkPos, coordinate::BlockPos(x, z));
			getBlock(x, z)->setPosition(block_position);
		}

		static coordinate::ChunkPos toChunkSettings(BlockPosT blockPosX, BlockPosT blockPosZ) {
			coordinate::ChunkPos chunk_settings;
			if (blockPosX / 16 == 0) {
				if (blockPosX < 0) chunk_settings.chunkPos = -1;
				else chunk_settings.chunkPos = 0;
			} else {
				if (blockPosX < 0) chunk_settings.chunkPos = blockPosX / 16 - 1;
				else chunk_settings.chunkPos = blockPosX / 16;
			}
			if (blockPosX < 0) {
				auto x = blockPosX - ChunkGenSettings::CHUNK_WIDTH * chunk_settings.chunkPos;
				if (x == 16) x = 0;
				chunk_settings.blockPos.x = x;
				chunk_settings.blockPos.z = blockPosZ;
			} else {
				chunk_settings.blockPos.x = blockPosX % 16;
				chunk_settings.blockPos.z = blockPosZ;;
			}
			return chunk_settings;
		}

		static coordinate::BlockPos toBlockPosition(ChunkPosT chunkPos, const coordinate::BlockPos& chunkBlockPos) {
			coordinate::BlockPos position{chunkBlockPos.x + ChunkGenSettings::CHUNK_WIDTH * chunkPos, chunkBlockPos.z};
			return position;
		}

		static PixelPosT convertToPixelPos(ChunkPosT chunkPos) {
			auto zoom = RenderSystem::Settings::pixelProportion;
			return chunkPos * zoom * ChunkGenSettings::CHUNK_WIDTH;
		}

		static ChunkPosT convertToChunkPos(PixelPosT pixelPos) {
			auto zoom = RenderSystem::Settings::pixelProportion;
			return (int) (pixelPos / (double) (zoom * ChunkGenSettings::CHUNK_WIDTH));
		}

		[[nodiscard]]
		ChunkPosT getChunkPosition() const { return chunkPos; }

		void update() {
			// TODO
		}

		void render() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					RenderSystem::render(*getBlockSprite(x_pos, y_pos));
				}
			}
		}

	private:
		friend class adapter::ChunkDataPacketAdapter;

		ChunkPosT chunkPos = -1;
		block::Block* chunkBlocks[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT]{};

		[[nodiscard]]
		sf::Sprite* getBlockSprite(ChunkPosT x, ChunkPosT z) const {
			return getBlock(x, z)->getSprite();
		}

		void setBlock(BlockPosT x, BlockPosT z, block::Block* block) {
			chunkBlocks[x][z] = block;
		}

		Chunk() = default;
	};
}


#endif //MINECRAFT_CHUNK_HPP
