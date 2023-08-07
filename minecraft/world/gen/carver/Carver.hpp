//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_CARVER_HPP
#define MINECRAFT_CARVER_HPP

#include "util/math/noise/PerlinNoise.hpp"
#include "block/attributes/Block.hpp"
#include "world/chunk/Chunk.hpp"

class Carver {
private:
	using ChunkPosT = coordinate::ChunkPositionT;
	using BlockPosT = coordinate::BlockPositionT;
protected:
public:
	explicit Carver(unsigned int seed) {
		noise.initialize(seed);
	}

	void curve(int (blockBuffer)[chunk::ChunkGenSettings::CHUNK_WIDTH][chunk::ChunkGenSettings::CHUNK_HEIGHT], BlockPosT blockPosX, BlockPosT chunkBlockPosX,
	           BlockPosT chunkBlockPos,
	           int curveThresholdConstant, bool condition) {
		double x = (double) blockPosX / ((double) 100);
		double y = (double) chunkBlockPos / ((double) 100);
		double n = 2 * noise.noise(x * 5, y * 5, 0.8);
		auto grey_channel = (uint8_t) floor(255 * n);
		if (grey_channel > curveThresholdConstant && condition)
			blockBuffer[chunkBlockPosX][chunkBlockPos] = block::blocks::Blocks::getInstance()->getBlockInstance("air_block")->getID().serialID;
	}

	~Carver() = default;

private:
	math::PerlinNoise noise;
};

#endif //MINECRAFT_CARVER_HPP
