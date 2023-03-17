//
// Created by robcholz on 1/20/23.
//

#ifndef RUNCRAFT_MATH_HELPER_HPP
#define RUNCRAFT_MATH_HELPER_HPP

#include <cmath>
#include <random>

float Random(int from, int to) {
    return from + (rand() % to);
}

template<typename vT=double>
class Vector2D {
private:
    vT x;
    vT y;
public:
    explicit Vector2D(vT x = 0.0f, vT y = 0.0f) {
        this->x = x;
        this->y = y;
    }

    Vector2D(const Vector2D &that) {
        this->x = that.x;
        this->y = that.y;
    }

    ~Vector2D() = default;

    vT getX() { return this->x; }

    vT getY() { return this->y; }

    Vector2D operator-(const Vector2D &that) {
        Vector2D result;
        result.x = this->x - that.x;
        result.y = this->y - that.y;
        return result;
    }

    Vector2D operator+(const Vector2D &that) {
        Vector2D result;
        result.x = this->x + that.x;
        result.y = this->y + that.y;
        return result;
    }

    void operator=(const Vector2D &that) {
        this->x = that.x;
        this->y = that.y;
    }
};

template<typename vT=double>
class Vector3D {
private:
    vT x;
    vT y;
    vT z;
public:
    explicit Vector3D(vT x = 0.0f, vT y = 0.0f, vT z = 0.0f) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3D(const Vector3D &that) {
        this->x = that.x;
        this->y = that.y;
        this->z = that.z;
    }

    ~Vector3D() = default;

    vT getX() { return this->x; }

    vT getY() { return this->y; }

    vT getZ() { return this->z; }

    Vector3D operator-(Vector3D that) {
        Vector3D result;
        result.x = this->x - that.x;
        result.y = this->y - that.y;
        result.z = this->z - that.z;
        return result;
    }

    Vector3D operator+(const Vector3D &that) {
        Vector3D result;
        result.x = this->x + that.x;
        result.y = this->y + that.y;
        result.z = this->z + that.z;
        return result;
    }

    void operator=(const Vector3D &that) {
        this->x = that.x;
        this->y = that.y;
        this->z = that.z;
    }
};

#endif //RUNCRAFT_MATH_HELPER_HPP
