// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "types.h"
#include <cmath>
#include <cassert>

namespace coffee {

f32 constexpr comparison_tolerance{ 1e-4 };
f32 constexpr pi{ 3.14159265359f };

inline f32 constexpr sqrt_f32(f32 value)
{
    return std::sqrtf(value);
}

inline f32 constexpr cos_f32(f32 value)
{
    return std::cosf(value);
}

inline f32 constexpr sin_f32(f32 value)
{
    return std::sinf(value);
}

inline f32 constexpr to_radians(f32 degrees)
{
    return degrees * pi / 180.0f;
}

struct Vector2
{
    Vector2(f32 x, f32 y) : _x{ x }, _y{ y }
    {
    }

    f32 _x;
    f32 _y;
};

struct Quaternion
{
    Quaternion() : _w{ 1.0f }, _x{ 0.0f }, _y{ 0.0f }, _z{ 0.0f }
    {
    }

    Quaternion(f32 w, f32 x, f32 y, f32 z) : _w{ w }, _x{ x }, _y{ y }, _z{ z }
    {
    }

    Quaternion operator*(Quaternion const& other) const
    {
        return Quaternion(
          _w * other._w - _x * other._x - _y * other._y - _z * other._z,
          _w * other._x + _x * other._w + _y * other._z - _z * other._y,
          _w * other._y - _x * other._z + _y * other._w + _z * other._x,
          _w * other._z + _x * other._y - _y * other._x + _z * other._w);
    }

    void normalise()
    {
        f32 const magnitude = sqrtf(_w * _w + _x * _x + _y * _y + _z * _z);

        assert(magnitude > 1.0f);

        _w = _w / magnitude;
        _x = _x / magnitude;
        _y = _y / magnitude;
        _z = _z / magnitude;
    }

    f32 _w;
    f32 _x;
    f32 _y;
    f32 _z;
};

inline void
rotate_vector(Vector2& vector, Quaternion const& rotation)
{
    Quaternion const v(0.0f, vector._x, vector._y, 0.0f);
    Quaternion const inverse(rotation._w, -rotation._x, -rotation._y, -rotation._z);
    Quaternion const result = rotation * v * inverse;

    vector._x = result._x;
    vector._y = result._y;
}

}; // namespace coffee
