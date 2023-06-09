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
	using PixelPositonT = int;

	template<typename vT>
	struct CoordinateStruct { vT x, z; };

	template<typename vT>
	class Coordinate {
	private:
		struct CoordinateStruct { vT x, z; } coordinate{};

	public:
		explicit Coordinate(vT x = 0, vT z = 0) {
			coordinate.x = x;
			coordinate.z = z;
		}

		Coordinate& setCoordinate(vT x, vT z) {
			coordinate.x = x;
			coordinate.z = z;
			return *this;
		}

		Coordinate& offset(vT x, vT z) {
			coordinate.x += x;
			coordinate.z += z;
			return *this;
		}

		[[nodiscard]] vT getX() const { return coordinate.x; }

		[[nodiscard]] int getIntX() const { return (int) coordinate.x; }

		[[nodiscard]] vT getZ() const { return coordinate.z; }

		[[nodiscard]] int getIntZ() const { return (int) coordinate.z; }

	};
}

#endif //RUNCRAFT_COORDINATE_HPP
