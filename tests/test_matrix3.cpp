#include "maths.h"
#include <gtest/gtest.h>

using namespace coffee;

TEST(Matrix_Test, constructor) {
    Matrix3 matrix;

    ASSERT_FLOAT_EQ(matrix.get(0, 0), 1.0f);
    ASSERT_FLOAT_EQ(matrix.get(1, 1), 1.0f);
    ASSERT_FLOAT_EQ(matrix.get(2, 2), 1.0f);
}

TEST(Matrix_Test, set_matrix_translation) {
    Matrix3 matrix;
    Vector2 position(20.0f, 30.0f);

    matrix.set_translation(position);

    ASSERT_FLOAT_EQ(matrix.get(2, 0), position._x);
    ASSERT_FLOAT_EQ(matrix.get(2, 1), position._y);
    ASSERT_FLOAT_EQ(matrix.get(2, 2), 1.0f);
}

TEST(Matrix_Test, set_matrix_orientation) {
    Matrix3 matrix;
    Vector2 position(1.0f, 0.0f);
    f32 orientation_radians = to_radians(90.0f);

    matrix.set_orientation(orientation_radians);

    ASSERT_FLOAT_EQ(matrix.get(0, 0),  cos_f32(orientation_radians));
    ASSERT_FLOAT_EQ(matrix.get(0, 1), -sin_f32(orientation_radians));
    ASSERT_FLOAT_EQ(matrix.get(1, 0),  sin_f32(orientation_radians));
    ASSERT_FLOAT_EQ(matrix.get(1, 1),  cos_f32(orientation_radians));
}
