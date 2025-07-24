#include "physics.h"
#include "constants.h"

void
coffee::update(f32 delta_time, world& world)
{
    for (u64 i {0}; i < world._positions.size(); ++i)
    {
        world._positions[i].x += world._velocities[i].x * delta_time;
        world._positions[i].y += world._velocities[i].y * delta_time;

        // Wrap around (we're going to change this)
        if (world._positions[i].x < 0.0f) {
            world._positions[i].x = 0.0f;
        } else if (world._positions[i].x > window_width) {
            world._positions[i].x -= window_width;
        }

        if (world._positions[i].y < 0.0f) {
            world._positions[i].y = 0.0f;
        } else if (world._positions[i].y > window_height) {
            world._positions[i].y -= window_height;
        }
    }
}
