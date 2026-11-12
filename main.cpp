#include <iostream>
#include <vector>
#include <queue>
#include <string>

#include <syncstream>

#ifdef GCC
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include "config.h"
#include "rng.h"
#include "generator.h"
#include "log1.h"

using namespace std;

using Main = void();
void task0();
void task1();

Main *functions[] = {task0, task1};
size_t nFunctions = sizeof(functions) / sizeof(remove_extent_t<decltype(functions)>);

string inputFile = "input/input.txt";

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

#ifdef GCC
    pid_t pid = getpid();
    cout << "Process " << pid << endl;
#endif

    int task = nFunctions - 1;
    if (argc > 1)
    {
        int tmp = atoi(argv[1]);
        if (tmp >= 0 and tmp < (int)nFunctions)
            task = tmp;
        else
            cerr << "Невідомий номер задачі '" << argv[1] << "', запускаю задачу " << task << endl;
    }
    if (argc > 2)
        inputFile = argv[2];

    functions[task]();

    return 0;
}

static void printQueue(queue<int> &q)
{
    cout << "[";
    for (bool first = true; !q.empty(); first = false)
    {
        cout << (first ? "" : ", ") << q.front();
        q.pop();
    }
    cout << "]";
}


void task0()
{
    vector<string> files = {
        "input/err_bad_format.txt",
        "input/err_duplicate.txt",
        "input/err_missing_total.txt",
        "input/err_not_number.txt",
        "input/err_out_of_range.txt",
        "input/err_unknown_key.txt",
        "input/no_such_file.txt",
    };

    cout << endl;
    for (auto &fname : files)
    {
        try
        {
            loadConfig(fname);
            cout << fname << ": OK" << endl;
        }
        catch (const ConfigError &e)
        {
            cout << fname << ": " << e.what() << endl;
        }
    }
    cout << endl;
}


void task1()
{
    Config config;
    try
    {
        config = loadConfig(inputFile);
    }
    catch (const ConfigError &e)
    {
        cerr << "Помилка у вхідних даних (" << inputFile << "): " << e.what() << endl;
        return;
    }

    RandomNumbers rng(config.seed);
    queue<int> q;
    Task coroutine = generateNumbers(rng, q, config.total);

    cout << endl;
    cout << "Вхідний файл: " << inputFile << endl;
    cout << "total = " << config.total
         << ", seed = " << (config.seed ? to_string(*config.seed) : string("random")) << endl;
    cout << endl;

    size_t pauses = 0;
    while (true)
    {
        coroutine.resume();
        if (coroutine.done())
            break;

        ++pauses;
        osyncstream(*Logger::stream()) << "pause " << pauses << ": odd " << q.back() << endl;
        cout << "Пауза #" << pauses << " (непарне " << q.back() << "), черга: ";
        printQueue(q);
        cout << endl;
    }

    cout << "Сопрограма завершилась, залишок у черзі: ";
    printQueue(q);
    cout << endl;
    cout << endl;
    cout << "Пауз: " << pauses << endl;
    cout << endl;
}
