#include <random>

static u32 seed;
static std::mt19937 engine;
static std::uniform_real_distribution<f32> real_distribution;
static std::uniform_int_distribution<s32> integer_distribution;

void initialise_random_system(u32 s) {
    seed = s;
    engine = std::mt19937(seed);
}

f32 get_random_real_number(f32 left_endpoint, f32 right_endpoint_inclusive) {
    real_distribution.param(std::uniform_real_distribution<f32>::param_type(left_endpoint, right_endpoint_inclusive));
    return real_distribution(engine);
}

f32 get_random_integer_number(s32 left_endpoint, s32 right_endpoint_inclusive) {
    integer_distribution.param(std::uniform_int_distribution<s32>::param_type(left_endpoint, right_endpoint_inclusive));
    return integer_distribution(engine);
}
