#include "generator.h"
#include "awaiter.h"

using namespace std;

Task generateNumbers(RandomNumbers &rng, queue<int> &q, size_t total)
{
    for (size_t i = 0; i < total; ++i)
    {
        int value = rng.next();
        q.push(value);
        co_await OddAwaiter(value);
    }
}
