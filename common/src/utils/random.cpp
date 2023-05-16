//
// Created by T2SU on 2023/05/16.
//

#include "std_common.hpp"
#include "utils/random.hpp"

std::random_device seeder;

hl::random::random()
    : _gen(seeder())
{
}

hl::random::random(uint32_t seed)
    : _gen(seed)
{
}
