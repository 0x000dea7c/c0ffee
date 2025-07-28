// @TODO: the name of this file sucks!
#include "maths.h"
#include <gtest/gtest.h>

using namespace coffee;

TEST(Test_Linear_Algebra_Operations, rotate_vector) {
    Vector2 vector(1.0f, 0.0f);
    f32 orientation_radians = to_radians(90.0f);

    Vector2 rotated_vector = rotate_vector(vector, orientation_radians);

    ASSERT_NEAR(rotated_vector._x, 0.0f, comparison_tolerance);
    ASSERT_NEAR(rotated_vector._y, 1.0f, comparison_tolerance);
}
