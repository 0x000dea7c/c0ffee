// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "types.h"
#include <cassert>
#include <cmath>

namespace coffee {

f32 constexpr comparison_tolerance{ 1e-4 }; // @XXX: this is pretty bad.
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
    constexpr Vector2(f32 x, f32 y) : _x{ x }, _y{ y }
    {
    }

    f32 _x;
    f32 _y;
};

inline Vector2
rotate_vector(Vector2 const& input, f32 orientation_radians)
{
    // https://en.wikipedia.org/wiki/Rotation_(mathematics)
    return Vector2{ input._x * cos_f32(orientation_radians) - input._y * sin_f32(orientation_radians),
                    input._x * sin_f32(orientation_radians) + input._y * cos_f32(orientation_radians) };
}

struct Matrix3
{
    Matrix3() : _elements{ 1.0f, 0.0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f }
    {
    }

    void set_translation(Vector2 const& position)
    {
        // We assume that the main diagonal is set up correctly.
        set(2, 0, position._x);
        set(2, 1, position._y);
        set(2, 2, 1);
    }

    void set_orientation(f32 orientation_radians)
    {
        f32 const cos_theta = cos_f32(orientation_radians);
        f32 const sin_theta = sin_f32(orientation_radians);

        set(0, 0, cos_theta);
        set(0, 1, -sin_theta);
        set(1, 0, sin_theta);
        set(1, 1, cos_theta);
    }

    Vector2 transform_vertex(Vector2 const& vertex)
    {
        return Vector2(
            vertex._x * get(0, 0) + vertex._y * get(1, 0) + get(2, 0),
            vertex._x * get(0, 1) + vertex._y * get(1, 1) + get(2, 1)
        );
    }

    void set(u32 row, u32 column, f32 value)
    {
        assert(row >= 0 && row <= 2 && column >= 0 && column <= 2);
        _elements[row * 3 + column] = value;
    }

    f32 get(u32 row, u32 column) const
    {
        assert(row >= 0 && row <= 2 && column >= 0 && column <= 2);
        return _elements[row * 3 + column];
    }

    alignas(16) f32 _elements[9];
};

}; // namespace coffee
