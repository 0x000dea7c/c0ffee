// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-

#pragma once

#include "c0ffee.h"
#include <vector>

namespace coffee {

struct bhv_node
{
    aabb bounds;
    bhv_node* left;
    bhv_node* right;
    b32 is_leaf;
    id entity_id;
};

struct bvh
{
    bhv_node* root;
};

/// Builds the tree with the given AABBs.
void
bvh_build(bvh* tree, std::vector<aabb> const& aabbs);

}; // namespace coffee
