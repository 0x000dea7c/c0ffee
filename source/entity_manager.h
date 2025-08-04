// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "types.h"
#include <vector>

namespace coffee {

struct World;
struct Renderer_Data;
struct Add_Entity_World_Arguments;

class Entity_Manager {
    std::vector<u64> entities;
    std::vector<u64> free_ids;
    World& world;
    Renderer_Data& renderer_data;

public:
    Entity_Manager(World& world, Renderer_Data& renderer_data);

    void add_player(Add_Entity_World_Arguments const& arguments);

    void add_rock(Add_Entity_World_Arguments const& arguments);
};

};
