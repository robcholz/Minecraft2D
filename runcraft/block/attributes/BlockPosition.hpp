//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKPOSITION_HPP
#define RUNCRAFT_BLOCKPOSITION_HPP

#include <memory>
#include "util/MathHelper.hpp"
#include "world/poi/Coordinate.hpp"

namespace block {

	enum class BlockDirectionType : uint8_t { UP, DOWN, NORTH, SOUTH, IN, OUT };

	class BlockPosition {
	private:
		using BlockPosT = coordinate::BlockPositionT;
		using BlockPosVecT = Vector2D<BlockPosT>;
		coordinate::Coordinate<coordinate::BlockPositionT> blockPosition;
		BlockDirectionType direction;
	public:
		explicit BlockPosition(BlockPosT x, BlockPosT z, BlockDirectionType blockDirection = BlockDirectionType::UP) {
			setPosition(x, z);
			direction = blockDirection;
		}

		BlockDirectionType getDirection() { return direction; }

		void setPosition(BlockPosT x, BlockPosT z) {
			blockPosition.setCoordinate(x, z);
		}

		void setPosition(coordinate::Coordinate<BlockPosT> coordinate) {
			blockPosition.setCoordinate(coordinate.getX(), coordinate.getZ());
		}

		void setDirection(BlockDirectionType blockDirection) {
			direction = blockDirection;
		}

		BlockPosition &up() {
			direction = BlockDirectionType::UP;
			return *this;
		}

		BlockPosition &down() {
			direction = BlockDirectionType::DOWN;
			return *this;
		}

		BlockPosition &west() {
			direction = BlockDirectionType::NORTH;
			return *this;
		}

		BlockPosition &south() {
			direction = BlockDirectionType::SOUTH;
			return *this;
		}

		BlockPosition &in() {
			direction = BlockDirectionType::IN;
			return *this;
		}

		BlockPosition &out() {
			direction = BlockDirectionType::OUT;
			return *this;
		}

	};
}


#endif //RUNCRAFT_BLOCKPOSITION_HPP
