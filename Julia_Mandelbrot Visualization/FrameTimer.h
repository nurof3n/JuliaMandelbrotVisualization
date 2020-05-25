#pragma once
#include <chrono>

// custom class to measure frame times
class FrameTimer {
public:
	FrameTimer();
	long long Mark();
private:
	std::chrono::steady_clock::time_point last;
};