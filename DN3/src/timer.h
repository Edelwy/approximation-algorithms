
#pragma once

#include <chrono>
#include "solver.h"

class CTimer {
    std::chrono::high_resolution_clock::time_point mStartTime;

public:
    void start();

    double stop();
};