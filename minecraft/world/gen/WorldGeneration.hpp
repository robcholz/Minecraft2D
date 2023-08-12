//
// Created by robcholz on 5/3/23.
//

#ifndef MINECRAFT_WORLDGENERATION_HPP
#define MINECRAFT_WORLDGENERATION_HPP

#include "world/poi/Coordinate.hpp"
#include "world/chunk/Chunk.hpp"
#include "util/math/noise/PerlinNoise.hpp"
#include "world/gen/carver/Carver.hpp"
#include "world/gen/decorator/HeightDecorator.hpp"

class WorldGeneration {
private:
	using BlockPosT = coordinate::BlockPositionT;
	using ChunkPosT = coordinate::ChunkPositionT;
protected:
	void generateBlock(ChunkPosT chunkPos) {
		static math::PerlinNoise noise(this->seed);
		for (int x_pos = 0; x_pos < chunk::ChunkGenSettings::CHUNK_WIDTH; x_pos++) {
			for (int z_pos = 0; z_pos < chunk::ChunkGenSettings::CHUNK_HEIGHT; z_pos++) {
				chunkBlocksBuffer[x_pos][z_pos] = block::Blocks::getInstance().getObjectInstance("minecraft:air_block")->getSerialID();
				//heightDecorator
			}
		}
		for (auto x_pos = 0; x_pos < chunk::ChunkGenSettings::CHUNK_WIDTH; x_pos++) {
			auto x_world = (float) (chunk::ChunkGenSettings::CHUNK_WIDTH * chunkPos + x_pos);
			auto height = math::Math::floor(30 * noise.noise(x_world / 25.f, 0.9, 0.8) + 30 * noise.noise(x_world / 50.f, 0.9, 0.8));
			auto surfaceZ = height + baseHeight;
			chunkBlocksBuffer[x_pos][surfaceZ] = block::Blocks::getInstance().getObjectInstance("minecraft:grass_block")->getSerialID();
			for (auto z_pos = surfaceZ - 4; z_pos < surfaceZ; z_pos++) {
				chunkBlocksBuffer[x_pos][z_pos] = block::Blocks::getInstance().getObjectInstance("minecraft:dirt_block")->getSerialID();
			}
			for (auto z_base = DEEPSLATE_BASE_HEIGHT; z_base < surfaceZ - 4; z_base++) {
				chunkBlocksBuffer[x_pos][z_base] = block::Blocks::getInstance().getObjectInstance("minecraft:stone_block")->getSerialID();
			}
			for (auto z_base = BEDROCK_BASE_HEIGHT; z_base < DEEPSLATE_BASE_HEIGHT; z_base++) {
				chunkBlocksBuffer[x_pos][z_base] = block::Blocks::getInstance().getObjectInstance("minecraft:deepslate_block")->getSerialID();
			}

			for (unsigned int z = 0; z < chunk::ChunkGenSettings::CHUNK_HEIGHT; ++z) {
				carver->curve(chunkBlocksBuffer, x_world, x_pos, z, 150,
				              [](BlockPosT z) { return z <= DEEPSLATE_BASE_HEIGHT; });
				carver->curve(chunkBlocksBuffer, x_world, x_pos, z, 200,
							  [](BlockPosT z) { return z > DEEPSLATE_BASE_HEIGHT; });
			}

			for (auto z_base = 0; z_base < BEDROCK_BASE_HEIGHT; z_base++) {
				chunkBlocksBuffer[x_pos][z_base] = block::Blocks::getInstance().getObjectInstance(block::Blocks::getInstance().BEDROCK_BLOCK)->getSerialID();
			}
		}
	}

public:

	explicit WorldGeneration(unsigned int seed) {
		this->seed = seed;
		baseHeight = chunk::ChunkGenSettings::GROUND_LEVEL + 50;
		carver = std::make_unique<Carver>(seed);
		heightDecorator=std::make_unique<HeightDecorator>(seed);
	}

	chunk::Chunk* getChunk(ChunkPosT chunkPos) {
		block::Block* blocks[chunk::ChunkGenSettings::CHUNK_WIDTH][chunk::ChunkGenSettings::CHUNK_HEIGHT];
		generateBlock(chunkPos);
		for (auto x_pos = 0; x_pos < chunk::ChunkGenSettings::CHUNK_WIDTH; x_pos++) {
			for (auto z_pos = 0; z_pos < chunk::ChunkGenSettings::CHUNK_HEIGHT; z_pos++) {
				auto block_serial_id = chunkBlocksBuffer[x_pos][z_pos];
				auto block=block::Blocks::getInstance().createObject(block_serial_id);
				blocks[x_pos][z_pos]=block;
			}
		}
		auto chunk = new chunk::Chunk(chunkPos, &blocks);
		return chunk;
	}

private:
	unsigned int seed;
	BlockPosT baseHeight;

	static constexpr int DEEPSLATE_BASE_HEIGHT = -chunk::ChunkGenSettings::CHUNK_BOTTOM_LIMIT;
	static constexpr int BEDROCK_BASE_HEIGHT = 5;

	std::unique_ptr<Carver> carver;
	std::unique_ptr<HeightDecorator> heightDecorator;

	block::ID::SerialIDT chunkBlocksBuffer[chunk::ChunkGenSettings::CHUNK_WIDTH][chunk::ChunkGenSettings::CHUNK_HEIGHT]{};
};

#endif //MINECRAFT_WORLDGENERATION_HPP
