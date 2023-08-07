//
// Created by robcholz on 5/13/23.
//

#ifndef MINECRAFT_CHUNKDATAPACKETADAPTER_HPP
#define MINECRAFT_CHUNKDATAPACKETADAPTER_HPP

#include <bitsery/traits/vector.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/bitsery.h>
#include <memory>
#include "Chunk.hpp"
#include "block/attributes/Blocks.hpp"

struct ChunkDataPacket {
	// note: since we know the position of the chunk and all the blocks in vector are stored linearly
	// we don't need to specify the position of each block
	uint32_t chunkPos; // the position of the chunk
	std::vector<uint32_t> serialIDContainer; // to identify the type of the block
	std::vector<uint8_t> blockDirectionContainer; // to identify the direction of the block
};

template<typename vT>
// serialization func implementation
void serialize(vT& s, ChunkDataPacket& o) {
	constexpr auto container_size = chunk::ChunkGenSettings::CHUNK_WIDTH * chunk::ChunkGenSettings::CHUNK_HEIGHT;
	s.value4b(o.chunkPos);
	s.container4b(o.serialIDContainer, container_size);
	s.container1b(o.blockDirectionContainer, container_size);
}

namespace chunk::adapter {
	class ChunkDataPacketAdapter {
	protected:
		using ChunkPosT = coordinate::ChunkPositionT;

		static ChunkPosT toOneDimensionIndex(ChunkPosT x, ChunkPosT y) { return y * ChunkGenSettings::CHUNK_WIDTH + x; }

	private:
		friend bitsery::Access;

		static void allocateContainerMemory(ChunkDataPacket* chunkDataPacket, ChunkPosT capacity) {
			chunkDataPacket->serialIDContainer.resize(capacity);
			chunkDataPacket->blockDirectionContainer.resize(capacity);
		}

		template<typename vT>
		static void addToContainer(std::vector<vT>* vector, ChunkPosT x, ChunkPosT y, vT value) {
			vector->at(toOneDimensionIndex(x, y)) = value;
		}

	public:
		ChunkDataPacketAdapter() = delete;

		ChunkDataPacketAdapter(const ChunkDataPacketAdapter&) = delete;

		~ChunkDataPacketAdapter() = delete;

		static void compress(Chunk* chunk, ChunkDataPacket* chunkDataPacket) {
			chunkDataPacket->chunkPos = chunk->chunkPos;
			// allocate the memory only once to save time
			allocateContainerMemory(chunkDataPacket, ChunkGenSettings::CHUNK_WIDTH * ChunkGenSettings::CHUNK_HEIGHT);
			for (ChunkPosT x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; x_pos++) {
				for (ChunkPosT y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; y_pos++) {
					auto block = chunk->chunkBlocks[x_pos][y_pos];
					addToContainer<uint32_t>(&chunkDataPacket->serialIDContainer, x_pos, y_pos, block->getID().serialID);
					addToContainer<uint8_t>(&chunkDataPacket->blockDirectionContainer, x_pos, y_pos,
					                        static_cast<uint8_t>(block->getPosition().getDirection().getDirection()));
				}
			}
		}

		static chunk::Chunk* decompress(ChunkDataPacket* chunkDataPacket) {
			auto chunk = new Chunk;
			auto chunkPos = static_cast<int32_t>(chunkDataPacket->chunkPos);
			chunk->chunkPos = chunkPos;
			for (ChunkPosT x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (ChunkPosT y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					auto oneDimensionIndex = toOneDimensionIndex(x_pos, y_pos);
					auto serialID = static_cast<int32_t>(chunkDataPacket->serialIDContainer[oneDimensionIndex]);
					auto block = block::blocks::Blocks::getInstance()->newBlock(serialID);
					chunk->setBlockPosition(x_pos, y_pos, block);
				}
			}
			return chunk;
		}
	};
}

#endif //MINECRAFT_CHUNKDATAPACKETADAPTER_HPP
