// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

#include "types.h"

struct OBB {
    OBB(f32 w, f32 h) : width(w), height(h) {}

    f32 width;
    f32 height;
};
