// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-

#pragma once

#include "maths.h"
#include "physics.h"

namespace coffee {

struct Renderer_Data
{
    std::vector<Matrix3> _transforms;

    Renderer_Data()
    {
        _transforms.reserve(1000);
    }

    void add_entity()
    {
        _transforms.emplace_back(Matrix3());
    }
};

/// TODO: improve this interface!
void
render(void* renderer, World& world, Renderer_Data& renderer_data);

};
