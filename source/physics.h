// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "maths.h"
#include "types.h"
#include <vector>

namespace coffee {

struct Add_Entity_World_Arguments
{
    Vector2 _position;
    Vector2 _velocity;
    Vector2 _acceleration;
    f32 _orientation_radians;
};

struct World
{
    World()
    {
        _positions.reserve(1000);
        _velocities.reserve(1000);
        _accelerations.reserve(1000);
        _orientations.reserve(1000);
    }

    void add_entity(Add_Entity_World_Arguments const& arguments)
    {
        _positions.emplace_back(arguments._position);
        _velocities.emplace_back(arguments._velocity);
        _accelerations.emplace_back(arguments._acceleration);
        _orientations.emplace_back(arguments._orientation_radians);
    }

    std::vector<Vector2> _positions;
    std::vector<Vector2> _velocities;
    std::vector<Vector2> _accelerations;
    std::vector<f32> _orientations;
};

void
update(f32 delta_time, World& world);

}; // namespace coffee
