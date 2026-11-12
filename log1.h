#pragma once
#include <fstream>
#include <string>

#include <mutex>
#include <memory>

class Logger
{
private:
    static const std::string fname;
    static std::once_flag initInstanceFlag;
    static std::shared_ptr<std::ofstream> f;
    constexpr Logger() = default;
    static void initLogger();

public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    constexpr ~Logger() = default;

    static std::shared_ptr<std::ofstream> stream();
};
