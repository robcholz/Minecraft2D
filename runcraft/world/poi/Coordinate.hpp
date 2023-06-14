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

	struct Acceleration{
		EntityPositionT x,z;
	};

	struct Velocity{
		EntityPositionT x,z;
	};

	struct EulerAngle{
		float x,y,z;
	};

	template<typename vT>
	struct CoordinateStruct { vT x, z; };

	template<typename vT>
	class Coordinate {
	private:
		struct CoordinateStruct { vT x, z; } coordinate{},offsetCoordinate{};

	public:
		explicit Coordinate(vT x = 0, vT z = 0) {
			coordinate.x = x;
			coordinate.z = z;
		}

		Coordinate& setCoordinate(vT x, vT z) {
			offsetCoordinate.x=x-coordinate.x;
			offsetCoordinate.z=z-coordinate.z;
			coordinate.x = x;
			coordinate.z = z;
			return *this;
		}

		Coordinate& offset(vT x, vT z) {
			setCoordinate(x+getCoordinate().x,z+getCoordinate().z);
			return *this;
		}

		[[nodiscard]] CoordinateStruct& getCoordinate() {
			return coordinate;
		}

		[[nodiscard]] CoordinateStruct& getOffset() {
			return offsetCoordinate;
		}

		[[nodiscard]] vT getX() const {return coordinate.x;}

		[[nodiscard]] int getIntX() const { return (int) coordinate.x; }

		[[nodiscard]] float getFloatX() const {return (float)coordinate.x;}

		[[nodiscard]] int getIntZ() const { return (int) coordinate.z; }

		[[nodiscard]] float getFloatZ() const {return (float)coordinate.z;}

		[[nodiscard]] vT getZ() const {return coordinate.z;}
	};
}

#endif //RUNCRAFT_COORDINATE_HPP
