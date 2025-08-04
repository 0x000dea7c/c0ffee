#include "physics.h"
#include "constants.h"

void World::update(f32 delta_time)
{
    for (u64 i = 0; i < positions.size(); ++i) {
        velocities[i].x *= 1.0 - damping * delta_time;
        velocities[i].y *= 1.0 - damping * delta_time;

        velocities[i].x += accelerations[i].x * delta_time;
        velocities[i].y += accelerations[i].y * delta_time;

        positions[i].x += velocities[i].x * delta_time;
        positions[i].y += velocities[i].y * delta_time;
    }
}
