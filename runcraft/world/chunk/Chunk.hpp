//
// Created by robcholz on 4/4/23.
//

#ifndef RUNCRAFT_CHUNK_HPP
#define RUNCRAFT_CHUNK_HPP

#include "world/poi/WorldCoordinate.hpp"
#include "block/Block.hpp"
#include "block/AirBlock.hpp"
#include "block/DirtBlock.hpp"

#define TEST_CHUNK_HEIGHT 20

namespace chunk {
	typedef std::function<void()> IteratorExecFuncPtr;
	typedef int in_chunk_block_pos_t;
	struct ChunkConfig {
		const static int CHUNK_HEIGHT = 320 + 64;
		const static int GROUND_LEVEL = 64;
		const static int CHUNK_WIDTH = 16;

		const static int CHUNK_UPPER_LIMIT = CHUNK_HEIGHT;
		const static int CHUNK_BOTTOM_LIMIT = 0;
	};

	struct ChunkPosT { coordinate::BlockPositionT x, y; };

	class Chunk {
	protected:
		static void chunkIterator(const IteratorExecFuncPtr &funcPtr) {
			for (int x_pos = 0; x_pos < ChunkConfig::CHUNK_WIDTH; ++x_pos)
				for (int y_pos = ChunkConfig::CHUNK_BOTTOM_LIMIT; y_pos < ChunkConfig::CHUNK_HEIGHT; ++y_pos)
					funcPtr();
		}
		static void chunkIterator(in_chunk_block_pos_t x_from, in_chunk_block_pos_t y_from, in_chunk_block_pos_t x_to, in_chunk_block_pos_t y_to,
		                          const IteratorExecFuncPtr &funcPtr) {
			for (int x_pos = x_from; x_pos < x_to; ++x_pos)
				for (int y_pos = y_from; y_pos < y_to; ++y_pos) funcPtr();
		}

	private:
		block::Block *(chunkBlocks[ChunkConfig::CHUNK_WIDTH][ChunkConfig::CHUNK_HEIGHT]){};

		block::Block *getBlock(ChunkPosT pos) { return chunkBlocks[pos.x][pos.y]; }

		static ChunkPosT transferBlockPosToChunkPos() {

		}

	public:
		explicit Chunk(){
			for (int x_pos = 0; x_pos < ChunkConfig::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = ChunkConfig::CHUNK_BOTTOM_LIMIT; y_pos < TEST_CHUNK_HEIGHT; ++y_pos) {
					replaceBlock(ChunkPosT{x_pos, y_pos}, new block::blocks::DirtBlock());
					getBlock(ChunkPosT{x_pos, y_pos})->getCurrentSprite()->setPosition(x_pos * 16, y_pos * 16);
				}
			}
		}

		~Chunk() {
			for (int x_pos = 0; x_pos < ChunkConfig::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = ChunkConfig::CHUNK_BOTTOM_LIMIT; y_pos < TEST_CHUNK_HEIGHT; ++y_pos) {
					delete getBlock(ChunkPosT{x_pos, y_pos});
				}
			}
		}

		void replaceBlock(ChunkPosT pos, block::Block *block) {
			if (chunkBlocks[pos.x][pos.y] != nullptr)
				delete chunkBlocks[pos.x][pos.y];
			chunkBlocks[pos.x][pos.y] = block;
		}

		void render() {
			//float prot=GameInfo.getConstExternalData()->windowState.zoomSize;
			//GameInfo.getExternalData()->windowState.camera->zoom(0.95f);
			//GameInfo.getRender()->getWindowConfig().window->setView(*GameInfo.getExternalData()->windowState.camera);
			for (int x_pos = 0; x_pos < ChunkConfig::CHUNK_WIDTH; ++x_pos) {
				for (int y_pos = ChunkConfig::CHUNK_BOTTOM_LIMIT; y_pos < TEST_CHUNK_HEIGHT; ++y_pos) {
					//getBlock(ChunkPosT{x_pos, y_pos})->getCurrentSprite()->setScale( prot/16.0f,  prot/16.0f);
					//getBlock(ChunkPosT{x_pos, y_pos})->getCurrentSprite()->setPosition(x_pos * prot, y_pos * prot);
					GameInfo.getRender()->render(*(getBlock(ChunkPosT{x_pos, y_pos})->getCurrentSprite()));
				}
			}
		}
	};
};


#endif //RUNCRAFT_CHUNK_HPP
