//
// Created by robcholz on 1/20/23.
//

#ifndef RUNCRAFT_MATHHELPER_HPP
#define RUNCRAFT_MATHHELPER_HPP

#pragma once

#include <cmath>
#include <random>

int Random(int from, int to) {
	return from + (rand() % to);
}

template<typename vT>
struct Area {
	vT x, y;
	vT width, height;
};
typedef Area<int> Areai;

template<typename vT>
struct Interval {
	vT lower, upper;
};
typedef Interval<int> Intervali;

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

	~Vector2D() = default;

	vT getX() { return this->i; }

	vT getY() { return this->j; }

	Vector2D operator-(const Vector2D &that) {
		Vector2D result;
		result.i = this->i - that.i;
		result.j = this->j - that.j;
		return result;
	}

	Vector2D operator+(const Vector2D &that) {
		Vector2D result;
		result.i = this->i + that.i;
		result.j = this->j + that.j;
		return result;
	}

	Vector2D &operator=(const Vector2D &that) {
		this->i = that.i;
		this->j = that.j;
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

	~Vector3D() = default;

	vT getX() { return this->i; }

	vT getY() { return this->j; }

	vT getZ() { return this->k; }

	Vector3D operator-(Vector3D that) {
		Vector3D result;
		result.i = this->i - that.i;
		result.j = this->j - that.j;
		result.k = this->k - that.k;
		return result;
	}

	Vector3D operator+(const Vector3D &that) {
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

typedef Vector3D<int> Vec3i;
typedef Vector3D<double> Vec3d;

#endif //RUNCRAFT_MATHHELPER_HPP
