//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKPOS_HPP
#define RUNCRAFT_BLOCKPOS_HPP

#include "util/MathHelper.hpp"
#include <memory>

enum BlockDirectionType : unsigned short { UP, DOWN, NORTH, SOUTH, IN, OUT };

class BlockPos {
private:
	std::shared_ptr<Vec3i *> blockCenterPos;
	BlockDirectionType direction;
public:
	explicit BlockPos(int i = 0, int j = 0, int k = 0) {
		blockCenterPos = std::make_shared<Vec3i *>(new Vec3i(i, j, k));
		direction = BlockDirectionType::UP;
	}

	BlockDirectionType getDirection() { return direction; }

	BlockPos &up() {
		direction = UP;
		return *this;
	}

	BlockPos &down() {
		direction = DOWN;
		return *this;
	}

	BlockPos &west() {
		direction = NORTH;
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
