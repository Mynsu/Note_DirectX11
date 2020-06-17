#include "timer.h"

Timer::Timer()
{
}


Timer::Timer(const Timer& other)
{
}


Timer::~Timer()
{
}

bool Timer::initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&mFrequency);
	if(mFrequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	mTicksPerMs = (float)(mFrequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&mStartTime);

	return true;
}

void Timer::frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - mStartTime);

	mFrameTime = timeDifference / mTicksPerMs;

	mStartTime = currentTime;

	return;
}

float Timer::getTime()
{
	return mFrameTime;
}