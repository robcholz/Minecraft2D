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

	class WorldCoordinate {
	private:
		BlockPositionT blockX, blockZ;
		WorldPositionT worldX, worldZ;

	public:
		explicit WorldCoordinate(WorldPositionT x, WorldPositionT z) {
			worldX = x;
			worldZ = z;
		}

		WorldCoordinate &offset(WorldPositionT x, WorldPositionT z) {
			worldX += x;
			worldZ += z;
			return *this;
		}

	};
}

#endif //RUNCRAFT_WORLDCOORDINATE_HPP
