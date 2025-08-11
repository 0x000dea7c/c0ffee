// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "types.h"

f32  constexpr player_thrust_force = 10000.0f;
f32  constexpr player_rotation_factor = 2.0f;
f32  constexpr damping = 0.5f;
f32  constexpr player_mass_kg = 100.0f;
s32  constexpr window_width = 1920;
s32  constexpr window_height = 1080;
f32  constexpr rock_spawn_safe_distance_to_player = 200.0f;
u64  constexpr game_maximum_entities = 1000;
char const*    game_title = "Coffee";
f32  constexpr fixed_delta_time = 1.0f / 60.0f;
f32  constexpr player_width = 50.0f;
f32  constexpr player_height = 35.0f;
f32  constexpr missile_width = 5.0f;
f32  constexpr missile_height = 15.0f;
