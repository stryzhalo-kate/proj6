#pragma once

#include <cstdint>
#include <optional>
#include <random>

class RandomNumbers
{
private:
    std::mt19937 engine;
    std::uniform_int_distribution<int> distribution;

public:
    static constexpr int minValue = 1;
    static constexpr int maxValue = 256;

    explicit RandomNumbers(std::optional<uint32_t> seed);
    RandomNumbers(const RandomNumbers &) = delete;
    RandomNumbers &operator=(const RandomNumbers &) = delete;

    int next();
};
