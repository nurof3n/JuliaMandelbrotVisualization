#pragma once

#include <chrono>

/**
 * @brief Custom class that measures frame times
 */
class FrameTimer {
public:
    FrameTimer() { last = std::chrono::steady_clock::now(); }
    /**
     * @brief Marks the time (in nanoseconds) in last
     *
     * @return the time passed since the last mark (in nanoseconds)
     */
    long long mark();

private:
    std::chrono::steady_clock::time_point last;  // holds the last marked time point
};