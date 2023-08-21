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
	coordinate::ChunkPositionT chunkPos; // the position of the chunk
	std::vector<block::ID::SerialIDT> serialIDContainer; // to identify the type of the block
	std::vector<Direction::DirectionT> blockDirectionContainer; // to identify the direction of the block
	std::vector<TileColor::TileColorT> tileColorContainer; // to store the light level
};

template<typename vT>
// serialization func implementation
void serialize(vT& s, ChunkDataPacket& o) {
	constexpr auto container_size = chunk::ChunkGenSettings::CHUNK_WIDTH * chunk::ChunkGenSettings::CHUNK_HEIGHT;
	s.value4b(o.chunkPos);
	s.container4b(o.serialIDContainer, container_size);
	s.container1b(o.blockDirectionContainer, container_size);
	s.container4b(o.tileColorContainer, container_size);
}

namespace chunk::adapter {
	class ChunkDataPacketAdapter {
	private:
		using Chunk = chunk::Chunk;
		using ChunkPosT = coordinate::ChunkPositionT;
		using SerialIDT = block::ID::SerialIDT;
		using DirectionT = Direction::DirectionT;
		using TileColorT = TileColor::TileColorT;

		static ChunkPosT toOneDimensionIndex(ChunkPosT x, ChunkPosT y) { return y * ChunkGenSettings::CHUNK_WIDTH + x; }

	private:
		friend bitsery::Access;

		static void allocateContainerMemory(ChunkDataPacket* chunkDataPacket, ChunkPosT capacity) {
			chunkDataPacket->serialIDContainer.resize(capacity);
			chunkDataPacket->blockDirectionContainer.resize(capacity);
			chunkDataPacket->tileColorContainer.resize(capacity);
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
					addToContainer<SerialIDT>(&chunkDataPacket->serialIDContainer, x_pos, y_pos, block->getSerialID());
					addToContainer<DirectionT>(&chunkDataPacket->blockDirectionContainer, x_pos, y_pos,
					                           static_cast<Direction::DirectionT>(block->getPosition().getDirection().getDirection()));
					addToContainer<TileColorT>(&chunkDataPacket->tileColorContainer, x_pos, y_pos,
					                           chunk->getRGBA(x_pos, y_pos));
				}
			}
		}

		static chunk::Chunk* decompress(ChunkDataPacket* chunkDataPacket) {
			auto chunkPos = static_cast<int32_t>(chunkDataPacket->chunkPos);
			block::Block* blocks[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT];
			Direction::DirectionT directions[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT];
			TileColor::TileColorT tile_colors[ChunkGenSettings::CHUNK_WIDTH][ChunkGenSettings::CHUNK_HEIGHT];
			for (ChunkPosT x_pos = 0; x_pos < ChunkGenSettings::CHUNK_WIDTH; ++x_pos) {
				for (ChunkPosT y_pos = 0; y_pos < ChunkGenSettings::CHUNK_HEIGHT; ++y_pos) {
					auto oneDimensionIndex = toOneDimensionIndex(x_pos, y_pos);
					auto serialID = static_cast<block::ID::SerialIDT>(chunkDataPacket->serialIDContainer[oneDimensionIndex]);
					auto block = block::Blocks::getInstance().createObject(serialID);
					blocks[x_pos][y_pos] = block;
					directions[x_pos][y_pos] = chunkDataPacket->blockDirectionContainer[oneDimensionIndex];
					tile_colors[x_pos][y_pos] = chunkDataPacket->tileColorContainer[oneDimensionIndex];
				}
			}
			auto chunk = new Chunk(chunkPos, &blocks,&directions, &tile_colors);
			return chunk;
		}
	};
}

#endif //MINECRAFT_CHUNKDATAPACKETADAPTER_HPP
