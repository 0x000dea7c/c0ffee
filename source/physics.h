// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include "types.h"
#include "math.h"

namespace coffee {

struct World
{
    World()
    {
        _positions.reserve(1000);
        _velocities.reserve(1000);
        _rotations.reserve(1000);
    }

    void
    add_entity(Vector2 position, Vector2 velocity, Quaternion const& rotation)
    {
        if (_positions.size() == 1000) {
            std::cerr << "Exceeded maximum entities in the game\n";
            std::exit(1);
        }

        _positions.emplace_back(position);
        _velocities.emplace_back(velocity);
        _rotations.emplace_back(rotation);
    }

    std::vector<Vector2> _positions;
    std::vector<Vector2> _velocities;
    std::vector<Quaternion> _rotations;
};

void
update(f32 delta_time, World& world);

}; // namespace physics
