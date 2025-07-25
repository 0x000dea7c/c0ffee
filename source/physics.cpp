#include "physics.h"

void
coffee::update(f32 delta_time, World& world)
{
    for (u64 i {0}; i < world._positions.size(); ++i)
    {
        world._positions[i]._x += world._velocities[i]._x * delta_time;
        world._positions[i]._y += world._velocities[i]._y * delta_time;
    }
}
