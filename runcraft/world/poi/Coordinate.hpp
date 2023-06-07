//
// Created by robcholz on 4/4/23.
//

#ifndef RUNCRAFT_COORDINATE_HPP
#define RUNCRAFT_COORDINATE_HPP

#include <vector>
#include <memory>

namespace coordinate {
	using EntityPositionT = float;
	using BlockPositionT = int;
	using ChunkPositionT = int;

	template<typename vT>
	class Coordinate {
	private:
		struct CoordinateStruct { vT x, z; } coordinate{};

	public:
		explicit Coordinate(vT x = 0, vT z = 0) {
			coordinate.x = x;
			coordinate.z = z;
		}

		void setCoordinate(vT x, vT z) {
			coordinate.x = x;
			coordinate.z = z;
		}

		[[nodiscard]] vT getX() const { return coordinate.x; }

		[[nodiscard]] int getIntX() const { return (int) coordinate.x; }

		[[nodiscard]] vT getZ() const { return coordinate.z; }

		[[nodiscard]] int getIntZ() const { return (int) coordinate.z; }

	};
}

#endif //RUNCRAFT_COORDINATE_HPP
