//
// Created by robcholz on 4/4/23.
//

#ifndef RUNCRAFT_CHUNK_HPP
#define RUNCRAFT_CHUNK_HPP

#include "bitsery/brief_syntax.h"
#include "world/poi/Coordinate.hpp"
#include "block/attributes/Block.hpp"
#include "block/attributes/BlockPosition.hpp"
#include "block/GrassBlock.hpp"
#include "block/IronBlock.hpp"
#include "block/AirBlock.hpp"

#define TEST_CHUNK_HEIGHT 6
#define TEST_BOTTOM 0

namespace chunk {
	namespace adapter {class ChunkDataPacketAdapter;}

	struct ChunkGenSettings {
		const static int CHUNK_HEIGHT = 16;
		const static int GROUND_LEVEL = 0;
		const static int CHUNK_WIDTH = 16;
		const static int CHUNK_UPPER_LIMIT = CHUNK_HEIGHT;
		const static int CHUNK_BOTTOM_LIMIT = 0;
	};// chunk configurations

	struct ChunkCoordinate {
		coordinate::ChunkPositionT chunkPos;
		coordinate::Coordinate<coordinate::BlockPositionT> blockPos{0, 0};
	};// the position of the block inside a chunk

	struct InChunkBlockPos { coordinate::BlockPositionT x, z; };

	class Chunk {
	protected:
		void onInitialize() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; x_pos++) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; y_pos++) {
					if (y_pos < TEST_CHUNK_HEIGHT)
						if (y_pos == TEST_CHUNK_HEIGHT - 1) {
							setBlockPosition(InChunkBlockPos{x_pos, y_pos}, new block::blocks::GrassBlock);
						} else {
							setBlockPosition(InChunkBlockPos{x_pos, y_pos}, new block::blocks::IronBlock);
						}
					else
						setBlockPosition(InChunkBlockPos{x_pos, y_pos}, new block::blocks::AirBlock);
				}
			}
		}

		void onRender() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					if (isBlockExisted(InChunkBlockPos{x_pos, y_pos}))
						GameInfo.getRender()->render(*getBlockSprite(InChunkBlockPos{x_pos, y_pos}));
				}
			}
		}

	private:
		using ChunkPosT = coordinate::ChunkPositionT;
		using BlockPosT = coordinate::BlockPositionT;
		using BlockCoordinate = coordinate::Coordinate<BlockPosT>;

		friend class adapter::ChunkDataPacketAdapter;

		ChunkPosT chunkPos{};
		block::Block* chunkBlocks[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT]{};

		// the chunk position
		// the block stored inside a chunk
		/**
		 * @param pos The block position
		 * @return The block object at that position
		 */
		block::Block* getBlock(InChunkBlockPos pos) { return chunkBlocks[pos.x][pos.z]; }

		block::Block* getBlock(BlockPosT x, BlockPosT y) { return chunkBlocks[x][y]; }

		bool isBlockExisted(InChunkBlockPos blockPos) { return getBlock(blockPos) != nullptr; }

		/**
		 * @param pos The block position
		 * @return The sprite of the block on that position
		 */
		sf::Sprite* getBlockSprite(InChunkBlockPos pos) { return getBlock(pos)->getSpritePtr(); }

		/**
		 * @param pos The position of the block
		 * @param block The block to set
		 */
		void setBlockPosition(InChunkBlockPos pos, block::Block* block) {
			if (getBlock(pos) != nullptr)
				delete getBlock(pos);
			chunkBlocks[pos.x][pos.z] = block;
			getBlock(pos)->getPositionPtr()->setPosition(toWorldCoordinateSettings(chunkPos, pos));
			setBlockOnScreenPosition(InChunkBlockPos{pos.x, pos.z});
		}

		/**
		 * @param pos The position of the block inside the chunk
		 */
		void setBlockOnScreenPosition(InChunkBlockPos pos) {
			auto height = GameInfo.getConstExternalData()->windowState.getScreenHeight();
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			getBlockSprite(pos)->setPosition((float) (chunkPos * zoom * ChunkGenSettings::CHUNK_WIDTH + pos.x * zoom),
			                                 (float) (height - zoom - pos.z * zoom));
		}

		explicit Chunk() = default;

	public:
		explicit Chunk(ChunkPosT chunkPos) {
			this->chunkPos = chunkPos;
			onInitialize();
		}

		~Chunk() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					delete getBlock(InChunkBlockPos{x_pos, y_pos});
				}
			}
		}

		/**
		 * @brief Convert from the world coordinate to the chunk coordinate
		 * @param worldCoordinate The world coordinate
		 * @return The coordinate parameters inside the chunk
		 */
		static ChunkCoordinate toChunkSettings(const coordinate::Coordinate<ChunkPosT> &worldCoordinate) {
			ChunkCoordinate chunk_settings{};
			chunk_settings.chunkPos = worldCoordinate.getIntX() / 16;
			chunk_settings.blockPos.setCoordinate(worldCoordinate.getIntX() % 16, worldCoordinate.getIntZ());
			return chunk_settings;
		}

		/**
		 * @brief Convert from the chunk coordinate to the world coordinate
		 * @param chunkCoordinate The chunk coordinate
		 * @return The world coordinate
		 */
		static coordinate::Coordinate<BlockPosT> toWorldCoordinateSettings(ChunkPosT chunkPos, InChunkBlockPos blockPos) {
			coordinate::Coordinate<BlockPosT> coordinate{blockPos.x + 16 * chunkPos, blockPos.z};
			return coordinate;
		}

		[[nodiscard]] ChunkPosT getChunkPosition() const { return chunkPos; }

		void render() {
			onRender();
		}
	};

}


#endif //RUNCRAFT_CHUNK_HPP
