
#pragma once

#include <chrono>
#include "solver.h"

class CTimer {
    clock_t::time_point mStartTime;

public:
    void start();

    double stop();
};

using clock_t = std::chrono::high_resolution_clock;