//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKPOS_HPP
#define RUNCRAFT_BLOCKPOS_HPP

#include "util/MathHelper.hpp"
#include <memory>

class BlockPos {
private:
	std::shared_ptr<Vec3i *> blockCenterPos;
	enum DirectionType : unsigned char { UP, DOWN, WEST, SOUTH, IN, OUT };
	DirectionType direction;
public:
	explicit BlockPos(int i, int j, int k) { blockCenterPos = std::make_shared<Vec3i *>(new Vec3i(i, j, k)); }

	DirectionType getDirection() { return direction; }

	BlockPos &up() {
		direction = UP;
		return *this;
	}

	BlockPos &down() {
		direction = DOWN;
		return *this;
	}

	BlockPos &west() {
		direction = WEST;
		return *this;
	}

	BlockPos &south() {
		direction = SOUTH;
		return *this;
	}

	BlockPos &in() {
		direction = IN;
		return *this;
	}

	BlockPos &out() {
		direction = OUT;
		return *this;
	}

};

#endif //RUNCRAFT_BLOCKPOS_HPP
