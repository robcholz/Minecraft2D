//
// Created by robcholz on 1/20/23.
//

#ifndef RUNCRAFT_MATHHELPER_HPP
#define RUNCRAFT_MATHHELPER_HPP

#include <cmath>
#include <random>

auto getRNG(int a, int b){
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(a,b);
	return dist6(rng);
}

template<typename vT>
struct Area {
	vT x, y;
	vT width, height;
};

template<typename vT>
struct Interval {
	vT lower, upper;
};

typedef Interval<int> Intervali;
typedef Area<int> Areai;

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
bool static approxEqual(vT value, vT val) {
	return (val + 1 > value) && (value > val - 1);
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

	Vector2D(const Vector2D &that) {
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

	Vector2D &operator-(const Vector2D &that) {
		Vector2D result;
		result.i = this->i - that.i;
		result.j = this->j - that.j;
		return result;
	}

	Vector2D &operator+(const Vector2D &that) {
		Vector2D result;
		result.i = this->i + that.i;
		result.j = this->j + that.j;
		return result;
	}

	Vector2D &operator=(const Vector2D &that) {
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

	Vector3D(const Vector3D &that) {
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

	Vector3D &operator-(Vector3D that) {
		Vector3D result;
		result.i = this->i - that.i;
		result.j = this->j - that.j;
		result.k = this->k - that.k;
		return result;
	}

	Vector3D &operator+(const Vector3D &that) {
		Vector3D result;
		result.i = this->i + that.i;
		result.j = this->j + that.j;
		result.k = this->k + that.k;
		return result;
	}

	Vector3D &operator=(const Vector3D &that) {
		this->i = that.i;
		this->j = that.j;
		this->k = that.k;
	}
};

typedef Vector2D<int> Vec2i;
typedef Vector2D<double> Vec2d;

typedef Vector3D<int> Vec3i;
typedef Vector3D<double> Vec3d;

#endif //RUNCRAFT_MATHHELPER_HPP
