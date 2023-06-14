//
// Created by robcholz on 4/4/23.
//

#ifndef RUNCRAFT_CHUNK_HPP
#define RUNCRAFT_CHUNK_HPP

#include "bitsery/brief_syntax.h"
#include "world/poi/Coordinate.hpp"
#include "block/attributes/Block.hpp"
#include "world/poi/Position.hpp"
#include "block/attributes/Blocks.hpp"

#define TEST_CHUNK_HEIGHT 13

namespace chunk {
	namespace adapter { class ChunkDataPacketAdapter; }

	struct ChunkGenSettings {
		const static int CHUNK_HEIGHT = 16;
		const static int GROUND_LEVEL = 0;
		const static int CHUNK_WIDTH = 16;
		const static int CHUNK_UPPER_LIMIT = CHUNK_HEIGHT;
		const static int CHUNK_BOTTOM_LIMIT = 0;
	};// chunk configurations

	struct ChunkCoordinate {
		coordinate::ChunkPositionT chunkPos;
		coordinate::Coordinate<coordinate::BlockPositionT> chunkBlockPos{0, 0};
	};// the position of the block inside a chunk

	using ChunkBlockCoordinate = coordinate::Coordinate<coordinate::ChunkPositionT>;

	class Chunk {
	protected:
		void onInitialize() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; x_pos++) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; y_pos++) {
					setBlockPosition(x_pos, y_pos, block::blocks::Blocks::getInstance()->newBlock("air_block"));
					if (y_pos < TEST_CHUNK_HEIGHT) {
						if (x_pos == 5)
							setBlockPosition(x_pos, 0, block::blocks::Blocks::getInstance()->newBlock("diamond_block"));
						else {
							if (y_pos == TEST_CHUNK_HEIGHT - 1)
								setBlockPosition(x_pos, y_pos, block::blocks::Blocks::getInstance()->newBlock("grass_block"));
							else
								setBlockPosition(x_pos, y_pos, block::blocks::Blocks::getInstance()->newBlock("cobblestone_block"));
						}
					}
				}
			}
		}

	private:
		using ChunkPosT = coordinate::ChunkPositionT;
		using BlockPosT = coordinate::BlockPositionT;
		using PixelPosT = coordinate::PixelPositonT;
		using BlockCoordinate = coordinate::Coordinate<BlockPosT>;

		friend class adapter::ChunkDataPacketAdapter;

		ChunkPosT chunkPos{};
		block::Block* chunkBlocks[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT]{};

		[[nodiscard]] sf::Sprite* getBlockSprite(ChunkBlockCoordinate chunkBlockPos) const { return getBlock(chunkBlockPos)->getSprite(); }

		[[nodiscard]] sf::Sprite* getBlockSprite(ChunkPosT x, ChunkPosT z) const { return getBlock(x, z)->getSprite(); }

		void setBlockPosition(const ChunkBlockCoordinate& chunkBlockPos, block::Block* block) {
			if (getBlock(chunkBlockPos) != nullptr)
				delete getBlock(chunkBlockPos);
			setBlock(chunkBlockPos.getX(), chunkBlockPos.getZ(), block);
			auto block_coordinate = toBlockCoordinate(chunkPos, chunkBlockPos);
			getBlock(chunkBlockPos)->getPosition()->setPosition(block_coordinate.getX(), block_coordinate.getZ());
		}

		void setBlockPosition(ChunkPosT x, ChunkPosT z, block::Block* block) {
			if (getBlock(x, z) != nullptr)
				delete getBlock(x, z);
			setBlock(x, z, block);
			auto block_coordinate = toBlockCoordinate(chunkPos, ChunkBlockCoordinate{x, z});
			getBlock(x, z)->getPosition()->setPosition(block_coordinate.getX(), block_coordinate.getZ());
		}

		Chunk() = default;

	public:
		explicit Chunk(ChunkPosT chunkPos) {
			this->chunkPos = chunkPos;
			onInitialize();
		}

		~Chunk() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					delete getBlock(ChunkBlockCoordinate{x_pos, y_pos});
				}
			}
		}

		[[nodiscard]] block::Block* getBlock(const ChunkBlockCoordinate& chunkBlockPos) const {
			return chunkBlocks[chunkBlockPos.getIntX()][chunkBlockPos.getIntZ()];
		}

		[[nodiscard]] block::Block* getBlock(BlockPosT x, BlockPosT z) const { return chunkBlocks[x][z]; }

		[[nodiscard]] block::Block* getBlockWithBoundaryCheck(const ChunkBlockCoordinate& chunkBlockPos) const {
			if ((chunkBlockPos.getIntZ() >= 0 && chunkBlockPos.getZ() < ChunkGenSettings::CHUNK_UPPER_LIMIT) &&
			    (chunkBlockPos.getIntX() >= 0 && chunkBlockPos.getIntX() < ChunkGenSettings::CHUNK_WIDTH))
				return getBlock(chunkBlockPos);
			return block::blocks::Blocks::getInstance()->getBlockInstance("error_block");
		}

		void setBlock(BlockPosT x, BlockPosT z, block::Block* block) { chunkBlocks[x][z] = block; }

		[[nodiscard]] bool isBlockExisted(const ChunkBlockCoordinate& coordinate) const { return getBlock(coordinate) != nullptr; }

		static ChunkCoordinate toChunkSettings(BlockPosT blockPosX, BlockPosT blockPosZ) {
			ChunkCoordinate chunk_settings{};
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
				chunk_settings.chunkBlockPos.setCoordinate(x, blockPosZ);
			} else chunk_settings.chunkBlockPos.setCoordinate(blockPosX % 16, blockPosZ);
			return chunk_settings;
		}

		static ChunkCoordinate toChunkSettings(const BlockCoordinate& chunkBlockPos) {
			return toChunkSettings(chunkBlockPos.getIntX(), chunkBlockPos.getIntZ());
		}

		static BlockCoordinate toBlockCoordinate(ChunkPosT chunkPos, const ChunkBlockCoordinate& chunkBlockPos) {
			coordinate::Coordinate<BlockPosT> coordinate{chunkBlockPos.getX() + ChunkGenSettings::CHUNK_WIDTH * chunkPos, chunkBlockPos.getZ()};
			return coordinate;
		}

		static PixelPosT convertToPixelPos(ChunkPosT chunkPos) {
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			return chunkPos * zoom * ChunkGenSettings::CHUNK_WIDTH;
		}

		static ChunkPosT convertToChunkPos(PixelPosT pixelPos) {
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			return (int) (pixelPos / (double) (zoom * ChunkGenSettings::CHUNK_WIDTH));
		}

		[[nodiscard]] ChunkPosT getChunkPosition() const { return chunkPos; }

		void update() {
			// TODO
		}

		void render() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					GameInfo.getRender()->render(*getBlockSprite(x_pos, y_pos));
					getBlock(x_pos, y_pos)->renderHitbox();
				}
			}
		}
	};
}


#endif //RUNCRAFT_CHUNK_HPP
