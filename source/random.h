// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
#pragma once

void initialise_random_system(u32 seed);

f32 get_random_real_number(f32 left_endpoint, f32 right_endpoint_inclusive);

f32 get_random_integer_number(s32 left_endpoint, s32 right_endpoint_inclusive);
