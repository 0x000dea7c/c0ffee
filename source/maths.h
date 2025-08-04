// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "types.h"
#include <cassert>
#include <cmath>
#include <random>

namespace coffee {

f32 constexpr comparison_tolerance{ 1e-4 }; // @FIXME: this is pretty bad.
f32 constexpr pi{ 3.14159265359f };

inline f32 constexpr square_f32(f32 value)
{
    return value * value;
}

struct Random_Generator
{
    Random_Generator(u32 seed)
        : _seed(seed)
    {
        _engine = std::mt19937(_seed);
    }

    f32 get_random_f32(f32 start, f32 end_inclusive) {
        _real_distribution.param(std::uniform_real_distribution<f32>::param_type(start, end_inclusive));
        return _real_distribution(_engine);
    }

    u64 get_random_s32(s32 start, s32 end_inclusive) {
        _integer_distribution.param(std::uniform_int_distribution<s32>::param_type(start, end_inclusive));
        return _integer_distribution(_engine);
    }

    u32 _seed;
    std::mt19937 _engine;
    std::uniform_real_distribution<f32> _real_distribution;
    std::uniform_int_distribution<s32> _integer_distribution;
};

inline f32 constexpr abs_f32(f32 value)
{
    return std::fabs(value);
}

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
    constexpr Vector2() : _x(0.0f), _y(0.0f) {}

    constexpr Vector2(f32 x, f32 y) : _x(x), _y(y) { }

    constexpr Vector2 operator+(Vector2 const& other) const {
        return {_x + other._x, _y + other._y};
    }

    constexpr Vector2 operator-(Vector2 const& other) const {
        return {_x - other._x, _y - other._y};
    }

    constexpr Vector2 operator*(Vector2 const& other) const {
        return {_x * other._x, _y * other._y};
    }

    constexpr Vector2 operator/(Vector2 const& other) const {
        return {_x / other._x, _y / other._y};
    }

    constexpr Vector2 operator*(f32 scale_factor) const {
        return {_x * scale_factor, _y * scale_factor};
    }

    void normalise() {
        f32 const magnitude = sqrt_f32(square_f32(_x) + square_f32(_y));
        _x /= magnitude;
        _y /= magnitude;
    }

    f32 _x;
    f32 _y;
};

inline f32 constexpr euclidean_distance_f32(Vector2 const& from, Vector2 const& to)
{
    return sqrt_f32(square_f32(to._x - from._x) + square_f32(to._y - from._y));
}

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
        return Vector2(vertex._x * get(0, 0) + vertex._y * get(1, 0) + get(2, 0),
                       vertex._x * get(0, 1) + vertex._y * get(1, 1) + get(2, 1));
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
