#include "FrameTimer.h"

long long FrameTimer::mark()
{
    const auto old = last;
    last           = std::chrono::steady_clock::now();
    return (last - old).count();
}
