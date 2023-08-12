//
// Created by robcholz on 1/20/23.
//

#ifndef MINECRAFT_MATHHELPER_HPP
#define MINECRAFT_MATHHELPER_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <random>
#include <boost/math/constants/constants.hpp>

namespace math {
	class Math {
	private:
		static const int SINE_TABLE_LENGTH = 65536;
		float SINE_TABLE[SINE_TABLE_LENGTH]{};
		double ARCSINE_TABLE[257]{};
		double COSINE_TABLE[257]{};
	public:
		static constexpr float PI = boost::math::constants::pi<float>();
		static constexpr float SQUARE_ROOT_OF_TWO = boost::math::constants::root_two<float>();
		static constexpr float SQUARE_ROOT_OF_THREE = boost::math::constants::root_three<float>();
		static constexpr float DEGREES_TO_RADIANS = (float) (boost::math::constants::pi<double>() / 180.0f);

		Math() {
			for (auto i = 0; i < SINE_TABLE_LENGTH; i++) {
				SINE_TABLE[i] = (float) std::sin((double) i * 2.0 * PI / 65536.0);
			}
		}

		float sin(float value) {
			return SINE_TABLE[(int) (value * 10430.378f) & 0xFFFF];
		}

		float cos(float value) {
			return SINE_TABLE[(int) (value * 10430.378f + 16384.0f) & 0xFFFF];
		}

		static int floor(float value) {
			int i = (int) value;
			return value < (float) i ? i - 1 : i;
		}

		static float ffloor(float value) {
			float i = value;
			return value < (float) i ? i - 1 : i;
		}

		static int fastFloor(double value) {
			return (int) (value + 1024.0) - 1024;
		}

		static int floor(double value) {
			int i = (int) value;
			return value < (double) i ? i - 1 : i;
		}

		static long lfloor(double value) {
			long l = (long) value;
			return value < (double) l ? l - 1L : l;
		}

		static float lerp(float t, float a, float b) {
			return a + t * (b - a);
		}

		static double lerp(double t, double a, double b) {
			return a + t * (b - a);
		}
	} Math;

	template<typename T>
	static T abs(T val) {
		return val < 0 ? -val : val;
	}

	template<typename T>
	static T clamp(T val, T min, T max) {
		return val < min ? min : (val > max ? max : val);
	}

	static int floor(float value) {
		int i = (int) value;
		return value < (float) i ? i - 1 : i;
	}

	static float ffloor(float value) {
		float i = value;
		return value < (float) i ? i - 1 : i;
	}

	static int fastFloor(double value) {
		return (int) (value + 1024.0) - 1024;
	}

	static int floor(double value) {
		int i = (int) value;
		return value < (double) i ? i - 1 : i;
	}

	static long lfloor(double value) {
		long l = (long) value;
		return value < (double) l ? l - 1L : l;
	}

	template<typename T>
	static T lerp(int t, int a, int b) {
		return a + t * (b - a);
	}

	static float lerp(float t, float a, float b) {
		return a + t * (b - a);
	}

	static double lerp(double t, double a, double b) {
		return a + t * (b - a);
	}

	template<typename T>
	static T map(T value, T inputMin, T inputMax, T outputMin, T outputMax) {
		value= clamp(value,inputMin, inputMax);
		float normalizedValue = (value - inputMin) / (inputMax - inputMin);
		float mappedValue = outputMin + (normalizedValue * (outputMax - outputMin));
		return floor(mappedValue);
	}
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
using Vec2f = Vector2D<float>;
using Vec2d = Vector2D<double>;

using Vec3i = Vector3D<int>;
using Vec3d = Vector3D<double>;

#endif //MINECRAFT_MATHHELPER_HPP
