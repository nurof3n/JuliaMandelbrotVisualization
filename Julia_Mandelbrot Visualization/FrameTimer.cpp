#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer() {
	last = steady_clock::now();
}
// mark the time in last and return the time passed since the last mark
float FrameTimer::Mark() {
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}
