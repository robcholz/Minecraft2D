//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKPOSITION_HPP
#define RUNCRAFT_BLOCKPOSITION_HPP

#include <memory>
#include "util/MathHelper.hpp"
#include "world/poi/Coordinate.hpp"
#include "world/poi/Direction.hpp"

namespace block {
	using BlockDirectionType = Direction::DirectionType;

	class BlockPosition : public Direction {
	private:
		using BlockPosT = coordinate::BlockPositionT;
		coordinate::Coordinate<coordinate::BlockPositionT> blockPosition;
	public:
		explicit BlockPosition(BlockPosT x, BlockPosT z, BlockDirectionType blockDirection = BlockDirectionType::UP) : Direction(blockDirection) {
			setPosition(x, z);
		}

		void setPosition(BlockPosT x, BlockPosT z) {
			blockPosition.setCoordinate(x, z);
		}

		void setPosition(coordinate::Coordinate<BlockPosT> coordinate) {
			blockPosition.setCoordinate(coordinate.getX(), coordinate.getZ());
		}
	};
}


#endif //RUNCRAFT_BLOCKPOSITION_HPP
