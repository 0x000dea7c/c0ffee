#include "physics.h"
#include "constants.h"

void
coffee::update(f32 delta_time, World& world)
{
    for (u64 i = 0; i < world._positions.size(); ++i) {
        world._velocities[i]._x *= 1.0 - damping * delta_time;
        world._velocities[i]._y *= 1.0 - damping * delta_time;

        world._velocities[i]._x += world._accelerations[i]._x * delta_time;
        world._velocities[i]._y += world._accelerations[i]._y * delta_time;

        world._positions[i]._x += world._velocities[i]._x * delta_time;
        world._positions[i]._y += world._velocities[i]._y * delta_time;
    }
}
