#pragma once
#include <windows.h>

class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool initialize();
	void frame();

	float getTime();

private:
	INT64 mFrequency;
	float mTicksPerMs;
	INT64 mStartTime;
	float mFrameTime;
};