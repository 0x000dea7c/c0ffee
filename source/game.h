// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include <vector>
#include "maths.h"
#include "collisions.h"

std::vector<Vector2> positions; // Centroid of the entity.
std::vector<Vector2> accelerations;
std::vector<Vector2> velocities;
std::vector<f32> orientations;
std::vector<OBB> sizes;
std::vector<Matrix3> transforms;

struct Add_Entity_Arguments {
    Matrix3 transform;
    OBB size;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    f32 orientation;
};

// The game checks is_pressed for continuous actions and transitions
// for discrete actions. Two half transitions mean a single press in a
// frame.
struct Button_State {
    Button_State() : is_pressed(false), half_transition(0) {}

    b32 is_pressed;
    u32 half_transition;
};

struct Player_Input {
    Button_State move_left;
    Button_State move_right;
    Button_State move_up;
    Button_State action_shoot;
};

struct Player {
    Player(Player_Input i) : input(i) {}

    Player_Input input;
};

void log(char const* fmt, ...);

void initialise();

void handle_input(f32 delta_time);

void update(f32 delta_time);

void render(f32 delta_time, void* renderer);

void add_entity(Add_Entity_Arguments const& args);

void remove_entity(u64 entity_id);

void draw_text(Vector2 const& position, char const* text, void* renderer);
