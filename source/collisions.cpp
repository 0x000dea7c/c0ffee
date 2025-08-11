#include "collisions.h"

static void project(OBB const& obb, Matrix3 transform, Vector2 const& axis, f32& min, f32& max) {
    // Project's an OBB's vertices onto an axis and returns min/max projections.
    Vector2 vertices[4] = {
        Vector2(-obb.width, -obb.height), // Bottom left.
        Vector2( obb.width, -obb.height), // Bottom right.
        Vector2( obb.width,  obb.height), // Top right.
        Vector2(-obb.width,  obb.height), // Top left.
    };

    min = max = transform.transform_vertex(vertices[0]).dot(axis);

    for(s32 i = 1; i < 4; ++i) {
        f32 const projection = transform.transform_vertex(vertices[i]).dot(axis);

        if(projection < min) {
            min = projection;
        }

        if(projection > max) {
            max = projection;
        }
    }
}

bool intersects(OBB const& a, Matrix3 const& a_transform, OBB const& b, Matrix3 const& b_transform) {
    // SAT (Separate Axis Theorem).
    // Get X and Y axes for both entities.
    Vector2 a_axes[2] = {
        a_transform.get_rotation() * Vector2(1.0f, 0.0f), // X-axis.
        a_transform.get_rotation() * Vector2(0.0f, 1.0f), // Y-axis.
    };

    Vector2 b_axes[2] = {
        b_transform.get_rotation() * Vector2(1.0f, 0.0f), // X-axis.
        b_transform.get_rotation() * Vector2(0.0f, 1.0f), // Y-axis.
    };

    // Test all four axes.
    for(s32 i = 0; i < 2; ++i) {
        f32 a_min;
        f32 a_max;
        f32 b_min;
        f32 b_max;

        project(a, a_transform, a_axes[i], a_min, a_max);
        project(b, b_transform, a_axes[i], b_min, b_max);

        if (a_max < b_min || b_max < a_min) {
            // Separating axis found, no collision.
            return false;
        }
    }

    for(s32 i = 0; i < 2; ++i) {
        f32 a_min;
        f32 a_max;
        f32 b_min;
        f32 b_max;

        project(a, a_transform, b_axes[i], a_min, a_max);
        project(b, b_transform, b_axes[i], b_min, b_max);

        if (a_max < b_min || b_max < a_min) {
            // Separating axis found, no collision.
            return false;
        }
    }

    // No separating axis found, it means they collided.
    return true;
}
