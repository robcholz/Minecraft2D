//
// Created by robcholz on 4/4/23.
//

#ifndef RUNCRAFT_WORLDCOORDINATE_HPP
#define RUNCRAFT_WORLDCOORDINATE_HPP

#include <vector>
#include <memory>
#include "world/chunk/Chunk.hpp"

namespace coordinate {

	typedef int BlockPositionT;
	typedef double WorldPositionT;

	struct Coordinate{WorldPositionT x, y;};

	class WorldCoordinate {
	private:
		Coordinate worldCoordinate;

	public:
		explicit WorldCoordinate(WorldPositionT x, WorldPositionT y) {
			worldCoordinate.x = x;
			worldCoordinate.y = y;
		}

		WorldCoordinate &offset(WorldPositionT x, WorldPositionT y) {
			worldCoordinate.x += x;
			worldCoordinate.y += y;
			return *this;
		}

		[[nodiscard]] WorldPositionT getX() const { return worldCoordinate.x; }

		[[nodiscard]] BlockPositionT getIntX() const { return (int) worldCoordinate.x; }

		[[nodiscard]] WorldPositionT getY() const { return worldCoordinate.y; }

		[[nodiscard]] BlockPositionT getIntY() const { return (int) worldCoordinate.y; }

	};
}

#endif //RUNCRAFT_WORLDCOORDINATE_HPP
