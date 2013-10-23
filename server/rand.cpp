#include "rand.hpp"

static std::random_device randomdev;
static std::mt19937 randgen(randomdev());

randgen_ret_type secure_rand()
{
    return randgen();
}
