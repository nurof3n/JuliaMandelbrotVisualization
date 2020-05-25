#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer() {
	last = steady_clock::now();
}
// mark the time (nanoseconds) in last and return the time passed since the last mark
long long FrameTimer::Mark() {
	const auto old = last;
	last = steady_clock::now();
	return (last - old).count();
}
