//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKPOS_HPP
#define RUNCRAFT_BLOCKPOS_HPP

#include "util/MathHelper.hpp"
#include "world/poi/WorldCoordinate.hpp"
#include <memory>

namespace block {

	enum BlockDirectionType : unsigned short { UP, DOWN, NORTH, SOUTH, IN, OUT };
	typedef coordinate::BlockPositionT BlockPositionT;
	typedef Vector2D<BlockPositionT> BlockPosVecT;

	class BlockPos {
	private:
		BlockPosVecT blockPosition;
		BlockDirectionType direction;
	public:
		explicit BlockPos(BlockPosVecT blockPos) {
			blockPosition=blockPos;
			setPosition(blockPos.getX(), blockPos.getY());
			direction = BlockDirectionType::UP;
		}

		BlockDirectionType getDirection() { return direction; }

		void setPosition(BlockPositionT x, BlockPositionT y){
			blockPosition.setVector(x, y);
		}

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
}


#endif //RUNCRAFT_BLOCKPOS_HPP
