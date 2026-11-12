#pragma once

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>

struct Config
{
    size_t total = 0;
    std::optional<uint32_t> seed;
};

class ConfigError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

constexpr size_t minTotal = 1;
constexpr size_t maxTotal = 100000;

Config loadConfig(const std::string &fname);
