#pragma once
#pragma comment(lib, "winmm.lib")

class FPS
{
public:
	FPS();
	FPS(const FPS&);
	~FPS();

	void initialize();
	void frame();
	int getFPS();

private:
	int mFPS, mCount;
	unsigned long mStartTime;
};