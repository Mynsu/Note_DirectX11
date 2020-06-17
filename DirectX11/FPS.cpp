#include "fps.h"
#include <windows.h>
//#include <mmsystem.h>

FPS::FPS()
{
}


FPS::FPS(const FPS& other)
{
}


FPS::~FPS()
{
}

void FPS::initialize()
{
	mFPS = 0;
	mCount = 0;
	mStartTime = timeGetTime();
	return;
}

void FPS::frame()
{
	mCount++;

	if(timeGetTime() >= (mStartTime + 1000))
	{
		mFPS = mCount;
		mCount = 0;

		mStartTime = timeGetTime();
	}
}

int FPS::getFPS()
{
	return mFPS;
}