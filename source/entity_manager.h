// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "types.h"
#include <vector>

namespace coffee {

struct World;
struct Renderer_Data;
struct Add_Entity_World_Arguments;

struct Entity_Manager
{
    Entity_Manager(World& world, Renderer_Data& renderer_data);

    std::vector<u64> _entities;
    World& _world;
    Renderer_Data& _renderer_data;
    u64 _current_entity_id;
    s32 _player_id;

    u64 get_player_id() const
    {
        return _entities[_player_id];
    }

    void add_player(Add_Entity_World_Arguments const& arguments);

    void add_rock(Add_Entity_World_Arguments const& arguments);
};

}; // namespace coffee
