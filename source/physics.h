// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "maths.h"
#include "types.h"
#include <vector>

namespace coffee {

struct World
{
    World()
    {
        _positions.reserve(1000);
        _velocities.reserve(1000);
        _accelerations.reserve(1000);
        _orientations.reserve(1000);
    }

    void add_entity(Vector2 position, Vector2 velocity, Vector2 acceleration, f32 orientation_radians)
    {
        assert(_positions.size() < 1000);

        _positions.emplace_back(position);
        _velocities.emplace_back(velocity);
        _accelerations.emplace_back(acceleration);
        _orientations.emplace_back(orientation_radians);
    }

    std::vector<Vector2> _positions;
    std::vector<Vector2> _velocities;
    std::vector<Vector2> _accelerations;
    std::vector<f32> _orientations;
};

void
update(f32 delta_time, World& world);

}; // namespace coffee
