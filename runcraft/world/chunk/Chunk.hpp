//
// Created by robcholz on 4/4/23.
//

#ifndef RUNCRAFT_CHUNK_HPP
#define RUNCRAFT_CHUNK_HPP

#include "world/poi/WorldCoordinate.hpp"
#include "block/Block.hpp"
#include "block/AirBlock.hpp"
#include "block/DirtBlock.hpp"
#include "block/BlockPos.hpp"

#define TEST_CHUNK_HEIGHT 100
#define TEST_BOTTOM 10
#define ZOOMING_PROT 16

namespace chunk {
	typedef std::function<void()> IteratorExecFuncPtr;
	typedef block::BlockPositionT ChunkPositionT;
	struct ChunkGenSettings {
		const static int CHUNK_HEIGHT = 320 + 64;
		const static int GROUND_LEVEL = 64;
		const static int CHUNK_WIDTH = 16;
		const static int CHUNK_UPPER_LIMIT = CHUNK_HEIGHT;
		const static int CHUNK_BOTTOM_LIMIT = 0;
	};
	struct ChunkSettings { ChunkPositionT chunkPos;block::BlockPositionT blockPosX, blockPosY; };
	struct BlockPos { coordinate::BlockPositionT x, y; };

	class Chunk {
	protected:
		static void chunkIterator(const IteratorExecFuncPtr &funcPtr) {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos)
				for (int y_pos = ChunkGenSettings::CHUNK_BOTTOM_LIMIT; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos)
					funcPtr();
		}

		void onInitialize() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = TEST_BOTTOM; y_pos < TEST_CHUNK_HEIGHT; ++y_pos) {
					setBlockPosition(BlockPos{x_pos, y_pos}, new block::blocks::DirtBlock());
					setBlockOnScreenPosition(BlockPos{x_pos, y_pos}, ZOOMING_PROT);
				}
			}
		}

		void onRender() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = TEST_BOTTOM; y_pos < TEST_CHUNK_HEIGHT; ++y_pos) {
					//getBlock(ChunkPosT{x_pos, y_pos})->getCurrentSprite()->setScale( prot/16.0f,  prot/16.0f);
					//getBlock(ChunkPosT{x_pos, y_pos})->getCurrentSprite()->setPosition(x_pos * prot, y_pos * prot);
					GameInfo.getRender()->render(*getBlockSprite(BlockPos{x_pos, y_pos}));
				}
			}
		}

	private:
		ChunkPositionT chunkPos;
		block::Block *(chunkBlocks[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT]){};

		block::Block *getBlock(BlockPos pos) { return chunkBlocks[pos.x][pos.y]; }

		sf::Sprite *getBlockSprite(BlockPos pos) { return getBlock(pos)->getCurrentSprite(); }

		void setBlockPosition(BlockPos pos, block::Block *block) {
			if (chunkBlocks[pos.x][pos.y] != nullptr)
				delete chunkBlocks[pos.x][pos.y];
			chunkBlocks[pos.x][pos.y] = block;
		}

		void setBlockOnScreenPosition(BlockPos pos, external_data::WindowState::ZoomT zoom) {
			getBlockSprite(pos)->setPosition(+(float) (toWorldCoordinateSettings(chunkPos) + pos.x * zoom), (float) (+pos.y * zoom));
		}

	public:
		explicit Chunk(ChunkPositionT chunkPos) {
			this->chunkPos = chunkPos;
			onInitialize();
		}

		~Chunk() {
			for (int x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = TEST_BOTTOM; y_pos < TEST_CHUNK_HEIGHT; ++y_pos) {
					delete getBlock(BlockPos{x_pos, y_pos});
				}
			}
		}

		static ChunkSettings toChunkSettings(const coordinate::WorldCoordinate &worldCoordinate) {
			ChunkSettings chunk_settings{};
			chunk_settings.chunkPos = worldCoordinate.getIntX() / 16;
			chunk_settings.blockPosX = worldCoordinate.getIntX() % 16;
			chunk_settings.blockPosY = worldCoordinate.getIntY();
			return chunk_settings;
		}

		static coordinate::Coordinate toWorldCoordinateSettings(const ChunkSettings &chunkSettings) {
			coordinate::Coordinate coordinate{};
			coordinate.x = chunkSettings.blockPosX + 16 * chunkSettings.chunkPos;
			coordinate.y = chunkSettings.blockPosY;
			return coordinate;
		}

		static coordinate::WorldPositionT toWorldCoordinateSettings(ChunkPositionT chunkPosition) {
			return chunkPosition * ZOOMING_PROT * ChunkGenSettings::CHUNK_WIDTH;
		}

		void render() {
			//float prot = GameInfo.getConstExternalData()->windowState.zoomSize;
			//GameInfo.getExternalData()->windowState.camera->setSize(prot*10,prot*10);
			GameInfo.getRender()->getWindowConfig().window->setView(*GameInfo.getExternalData()->windowState.camera);
			onRender();
		}
	};
};


#endif //RUNCRAFT_CHUNK_HPP
