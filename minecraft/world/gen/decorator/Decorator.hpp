//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_DECORATOR_HPP
#define MINECRAFT_DECORATOR_HPP

#include "util/math/noise/PerlinNoise.hpp"
#include "world/chunk/Chunk.hpp"

class Decorator {
protected:
	using BlockPosT = coordinate::BlockPositionT;
public:
	virtual void decorate(int (blockBuffer)[chunk::ChunkGenSettings::CHUNK_WIDTH][chunk::ChunkGenSettings::CHUNK_HEIGHT],
	                      BlockPosT blockPosX, BlockPosT chunkBlockPosX, BlockPosT chunkBlockPos, int curveThresholdConstant, bool condition) = 0;

private:
};

#endif //MINECRAFT_DECORATOR_HPP
