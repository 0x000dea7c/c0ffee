#include "math.h"
#include <gtest/gtest.h>

using namespace coffee;

class Quaternion_test : public ::testing::Test
{
  protected:
    Quaternion _identity;

    void SetUp() override
    {
        _identity = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    }
};

TEST_F(Quaternion_test, constructor)
{
    Quaternion q(1.0f, 25.0f, 26.0f, 27.0f);

    EXPECT_FLOAT_EQ(q._w, 1.0f);
    EXPECT_FLOAT_EQ(q._x, 25.0f);
    EXPECT_FLOAT_EQ(q._y, 26.0f);
    EXPECT_FLOAT_EQ(q._z, 27.0f);
}

TEST_F(Quaternion_test, normalise)
{
    Quaternion q(1.0f, 25.0f, 26.0f, 27.0f);

    q.normalise();

    EXPECT_NEAR(q._w, 0.0221893733909f, comparison_tolerance);
    EXPECT_NEAR(q._x, 0.554734334773f, comparison_tolerance);
    EXPECT_NEAR(q._y, 0.576923708164f, comparison_tolerance);
    EXPECT_NEAR(q._z, 0.599113081555f, comparison_tolerance);
}

TEST_F(Quaternion_test, multiply_by_identity)
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion result = q * _identity;

    EXPECT_NEAR(q._w, 1.0f, comparison_tolerance);
    EXPECT_NEAR(q._x, 2.0f, comparison_tolerance);
    EXPECT_NEAR(q._y, 3.0f, comparison_tolerance);
    EXPECT_NEAR(q._z, 4.0f, comparison_tolerance);
}

TEST_F(Quaternion_test, to_matrix)
{
}

TEST_F(Quaternion_test, rotate_vector)
{
    f32 angle = to_radians(90.0f);
    Vector2 vector(1.0f, 0.0f);
    Quaternion rotation(cos_f32(angle / 2.0f), 0.0f, 0.0f, sin_f32(angle / 2.0f));

    rotate_vector(vector, rotation);

    EXPECT_NEAR(vector._x, 0.0f, comparison_tolerance);
    EXPECT_NEAR(vector._y, 1.0f, comparison_tolerance);
}
