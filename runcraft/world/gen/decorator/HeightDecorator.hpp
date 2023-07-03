//
// Created by robcholz on 7/2/23.
//

#ifndef RUNCRAFT_HEIGHTDECORATOR_HPP
#define RUNCRAFT_HEIGHTDECORATOR_HPP


#include "Decorator.hpp"

class HeightDecorator: public Decorator{
private:
protected:
public:
	explicit HeightDecorator(unsigned int seed) {
		noise.initialize(seed);
	}

	void decorate(int (blockBuffer)[chunk::ChunkGenSettings::CHUNK_WIDTH][chunk::ChunkGenSettings::CHUNK_HEIGHT],
	              BlockPosT blockPosX, BlockPosT chunkBlockPosX, BlockPosT chunkBlockPos,
	              int curveThresholdConstant, bool condition) override {
		double x = (double) blockPosX / ((double) 100);
		double y = (double) chunkBlockPos / ((double) 100);
		double n = 2 * noise.noise(x * 5, y * 5, 0.8);
		auto grey_channel = (uint8_t) floor(255 * n);
		if (grey_channel > curveThresholdConstant && condition)
			blockBuffer[chunkBlockPosX][chunkBlockPos] = block::blocks::Blocks::getInstance()->getBlockInstance("air_block")->getID().serialID;
	}


private:
	math::PerlinNoise noise;
};

#endif //RUNCRAFT_HEIGHTDECORATOR_HPP
