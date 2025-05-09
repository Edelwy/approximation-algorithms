
#include "timer.h"

void CTimer::start() 
{
    mStartTime = std::chrono::high_resolution_clock::now();
}

double CTimer::stop() 
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(endTime - mStartTime).count();
    return duration;
}