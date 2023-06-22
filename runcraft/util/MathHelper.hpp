//
// Created by robcholz on 1/20/23.
//

#ifndef RUNCRAFT_MATHHELPER_HPP
#define RUNCRAFT_MATHHELPER_HPP

#include <cmath>
#include <random>

namespace math {
	class RandomNumGenerator {
	public:
		static auto getRange(int a, int b) {
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(a, b);
			return dist6(rng);
		}

		static bool randomBool() {
			return getRange(0, 1) == 1;
		}

		static bool randomBool(float probability) {
			return (getRange(1, (int) lroundf(1.f / probability)) == 1);
		}

	};
}

template<typename vT>
struct Area {
	vT x, y;
	vT width, height;
};

template<typename T>
struct IntRect {
	T x1, y1;
	T x2, y2;
};

template<typename vT>
struct Interval {
	vT lower, upper;
};

using Intervali = Interval<int>;
using Areai = Area<int>;
using IntRecti = IntRect<int>;

template<typename vT>
bool static checkIntervalBoundary(vT value, Interval<vT> interval) {
	return (value > interval.lower && value < interval.upper);
}

bool static checkVectorBoundary(sf::Vector2i vector, Areai area) {
	return ((vector.x > area.x && vector.x < area.x + area.width) &&
	        (vector.y > area.y && vector.y < area.y + area.height));
}

bool static checkVectorBoundary(int x, int y, int left, int top, int width, int height) {
	return ((x > left && x < left + width) && (y > top && y < top + height));
}

template<typename vT>
bool static approxEqual(vT v1, vT v2) {
	return (v2 + 1 > v1) && (v1 > v2 - 1);
}

template<typename vT=double>
class Vector2D {
private:
	vT i;
	vT j;
public:
	explicit Vector2D(vT i = 0.0f, vT j = 0.0f) {
		this->i = i;
		this->j = j;
	}

	Vector2D(const Vector2D& that) {
		this->i = that.i;
		this->j = that.j;
	}

	void setVector(vT i, vT j) {
		this->i = i;
		this->j = j;
	}

	~Vector2D() = default;

	vT getX() { return this->i; }

	vT getY() { return this->j; }

	Vector2D& operator-(const Vector2D& that) {
		Vector2D result;
		result.i = this->i - that.i;
		result.j = this->j - that.j;
		return result;
	}

	Vector2D& operator+(const Vector2D& that) {
		Vector2D result;
		result.i = this->i + that.i;
		result.j = this->j + that.j;
		return result;
	}

	Vector2D& operator=(const Vector2D& that) {
		this->i = that.i;
		this->j = that.j;
		return *this;
	}
};

template<typename vT=double>
class Vector3D {
private:
	vT i;
	vT j;
	vT k;
public:
	explicit Vector3D(vT i = 0.0f, vT j = 0.0f, vT k = 0.0f) {
		this->i = i;
		this->j = j;
		this->k = k;
	}

	Vector3D(const Vector3D& that) {
		this->i = that.i;
		this->j = that.j;
		this->k = that.k;
	}

	void setVector(vT i, vT j, vT k) {
		this->i = i;
		this->j = j;
		this->k = k;
	}

	~Vector3D() = default;

	vT getX() { return this->i; }

	vT getY() { return this->j; }

	vT getZ() { return this->k; }

	Vector3D& operator-(Vector3D that) {
		Vector3D result;
		result.i = this->i - that.i;
		result.j = this->j - that.j;
		result.k = this->k - that.k;
		return result;
	}

	Vector3D& operator+(const Vector3D& that) {
		Vector3D result;
		result.i = this->i + that.i;
		result.j = this->j + that.j;
		result.k = this->k + that.k;
		return result;
	}

	Vector3D& operator=(const Vector3D& that) {
		this->i = that.i;
		this->j = that.j;
		this->k = that.k;
	}
};

using Vec2i = Vector2D<int>;
using Vec2d = Vector2D<double>;

using Vec3i = Vector3D<int>;
using Vec3d = Vector3D<double>;

#endif //RUNCRAFT_MATHHELPER_HPP
