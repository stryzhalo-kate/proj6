#include "rng.h"

using namespace std;

RandomNumbers::RandomNumbers(optional<uint32_t> seed)
    : engine(seed ? *seed : random_device{}()), distribution(minValue, maxValue)
{
}

int RandomNumbers::next()
{
    return distribution(engine);
}
