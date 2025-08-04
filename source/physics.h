// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "maths.h"
#include "types.h"
#include <vector>

namespace coffee {

struct Add_Entity_World_Arguments {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    f32 orientation_radians;
};

class World {
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<Vector2> accelerations;
    std::vector<f32> orientations;

public:
    World() {
        positions.reserve(game_maximum_entities);
        velocities.reserve(game_maximum_entities);
        accelerations.reserve(game_maximum_entities);
        orientations.reserve(game_maximum_entities);
    }

    void add_entity(Add_Entity_World_Arguments const& arguments) {
        positions.emplace_back(arguments.position);
        velocities.emplace_back(arguments.velocity);
        accelerations.emplace_back(arguments.acceleration);
        orientations.emplace_back(arguments.orientation_radians);
    }

    void update(f32 delta_time);

    std::vector<Vector2> const& get_positions() const { return positions; }

    std::vector<Vector2> const& get_velocities() const { return velocities; }

    std::vector<Vector2> const& get_accelerations() const { return accelerations; }

    std::vector<f32> const& get_orientations() const { return orientations; }

    void set_orientation(u64 index, f32 orientation) {
        orientations[index] = orientation;
    }

    void set_acceleration(u64 index, Vector2 const& acceleration) {
        accelerations[index] = acceleration;
    }
};

};
