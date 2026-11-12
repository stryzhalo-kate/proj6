#include "log1.h"

using namespace std;

const string Logger::fname = "log.txt";
once_flag Logger::initInstanceFlag;
shared_ptr<ofstream> Logger::f;

void Logger::initLogger()
{
    f = shared_ptr<ofstream>(new ofstream(fname));
}

shared_ptr<ofstream> Logger::stream()
{
    call_once(initInstanceFlag, Logger::initLogger);
    return f;
}
