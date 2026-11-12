#include "config.h"

#include <charconv>
#include <fstream>
#include <limits>
#include <string_view>

using namespace std;

static string_view trim(string_view s)
{
    auto first = s.find_first_not_of(" \t\r\n");
    if (first == string_view::npos)
        return {};
    auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

static string lineError(size_t lineNo, const string &msg)
{
    return "рядок " + to_string(lineNo) + ": " + msg;
}

static unsigned long long parseNumber(string_view text, unsigned long long lo, unsigned long long hi,
                                      size_t lineNo, string_view key)
{
    if (text.empty())
        throw ConfigError(lineError(lineNo, "порожнє значення для '" + string(key) + "'"));

    unsigned long long value = 0;
    const char *end = text.data() + text.size();
    auto [ptr, ec] = from_chars(text.data(), end, value);

    if (ec == errc::result_out_of_range)
        throw ConfigError(lineError(lineNo, "значення '" + string(text) + "' занадто велике"));
    if (ec != errc() or ptr != end)
        throw ConfigError(lineError(lineNo, "'" + string(text) + "' не є цілим невід'ємним числом"));
    if (value < lo or value > hi)
        throw ConfigError(lineError(lineNo, string(key) + " = " + to_string(value) + " поза межами [" +
                                                to_string(lo) + ", " + to_string(hi) + "]"));
    return value;
}

Config loadConfig(const string &fname)
{
    ifstream in(fname);
    if (!in)
        throw ConfigError("не вдалося відкрити файл '" + fname + "'");

    Config config;
    bool hasTotal = false;
    string raw;
    size_t lineNo = 0;

    while (getline(in, raw))
    {
        ++lineNo;
        string_view line = raw;

        auto hash = line.find('#');
        if (hash != string_view::npos)
            line = line.substr(0, hash);
        line = trim(line);
        if (line.empty())
            continue;

        auto eq = line.find('=');
        if (eq == string_view::npos)
            throw ConfigError(lineError(lineNo, "очікується формат 'ключ = значення'"));

        string_view key = trim(line.substr(0, eq));
        string_view value = trim(line.substr(eq + 1));

        if (key == "total")
        {
            if (hasTotal)
                throw ConfigError(lineError(lineNo, "ключ 'total' задано повторно"));
            config.total = parseNumber(value, minTotal, maxTotal, lineNo, key);
            hasTotal = true;
        }
        else if (key == "seed")
        {
            if (config.seed)
                throw ConfigError(lineError(lineNo, "ключ 'seed' задано повторно"));
            config.seed = static_cast<uint32_t>(
                parseNumber(value, 0, numeric_limits<uint32_t>::max(), lineNo, key));
        }
        else if (key.empty())
            throw ConfigError(lineError(lineNo, "порожній ключ"));
        else
            throw ConfigError(lineError(lineNo, "невідомий ключ '" + string(key) + "'"));
    }

    if (!hasTotal)
        throw ConfigError("у файлі '" + fname + "' відсутній обов'язковий ключ 'total'");

    return config;
}
