// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include <cmath>

f32 constexpr comparison_tolerance = 1e-4;
f32 constexpr pi = 3.14159265359f;

inline f32 constexpr square(f32 value) {
    return value * value;
}

inline f32 constexpr absolute_value(f32 value) {
    return value < 0 ? -value : value;
}

inline f32 constexpr square_root(f32 value) {
    return std::sqrtf(value);
}

inline f32 constexpr cosine(f32 value) {
    return std::cosf(value);
}

inline f32 constexpr sine(f32 value) {
    return std::sinf(value);
}

inline f32 constexpr to_radians(f32 degrees) {
    return degrees * pi / 180.0f;
}

// ---------------------------------------------------------

struct Vector2 {
    constexpr Vector2() : x(0.0f), y(0.0f) {}
    constexpr Vector2(f32 x1, f32 y1) : x(x1), y(y1) {}

    constexpr Vector2 operator+(Vector2 const& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    constexpr Vector2 operator-(Vector2 const& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    constexpr Vector2 operator*(Vector2 const& other) const {
        return Vector2(x * other.x, y * other.y);
    }

    constexpr Vector2 operator*(f32 scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    constexpr Vector2 operator/(Vector2 const& other) const {
        return Vector2(x / other.x, y / other.y);
    }

    void normalise() {
        f32 const magnitude = sqrt(square(x) + square(y));
        x /= magnitude;
        y /= magnitude;
    }

    void rotate(f32 orientation) {
        f32 new_x = x * cosine(orientation) - y * sine(orientation);
        f32 new_y = x * sine(orientation) + y * cosine(orientation);
        x = new_x;
        y = new_y;
    }

    f32 dot(Vector2 const& other) const {
        return x * other.x + y * other.y;
    }

    f32 x;
    f32 y;
};

// ---------------------------------------------------------

inline f32 constexpr euclidean_distance(Vector2 const& from, Vector2 const& to) {
    return square_root(square(to.x - from.x) + square(to.y - from.y));
}

// To avoid computing the square root for quick computations.
inline f32 constexpr euclidean_distance_squared(Vector2 const& from, Vector2 const& to) {
    return square(to.x - from.x) + square(to.y - from.y);
}

struct Matrix3 {
    Matrix3() : _elements{1.0f, 0.0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f} {}

    Matrix3 get_rotation() const {
        Matrix3 rotation;
        rotation.set(0, 0, get(0, 0));
        rotation.set(0, 1, get(0, 1));
        rotation.set(1, 0, get(1, 0));
        rotation.set(1, 1, get(1, 1));
        return rotation;
    }

    void set_translation(Vector2 const& position) {
        // We assume that the main diagonal is set up correctly.
        set(2, 0, position.x);
        set(2, 1, position.y);
        set(2, 2, 1);
    }

    void set_orientation(f32 orientation_radians) {
        f32 const cos_theta = cosine(orientation_radians);
        f32 const sin_theta = sine(orientation_radians);

        set(0, 0,  cos_theta);
        set(0, 1, -sin_theta);
        set(1, 0,  sin_theta);
        set(1, 1,  cos_theta);
    }

    Vector2 transform_vertex(Vector2 const& vertex) {
        return Vector2(vertex.x * get(0, 0) + vertex.y * get(1, 0) + get(2, 0),
                       vertex.x * get(0, 1) + vertex.y * get(1, 1) + get(2, 1));
    }

    void set(u32 row, u32 column, f32 value) {
        _elements[row * 3 + column] = value;
    }

    f32 get(u32 row, u32 column) const {
        return _elements[row * 3 + column];
    }

    Vector2 operator*(Vector2 const& other) const {
        return Vector2(
            get(0, 0) * other.x + get(0, 1) * other.y,
            get(1, 0) * other.x + get(1, 1) * other.y);
    }

    alignas(16) f32 _elements[9];
};
