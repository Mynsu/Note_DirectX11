#pragma once
#pragma comment(lib, "pdh.lib")
#include <pdh.h>

class CPU
{
public:
	CPU();
	CPU(const CPU&);
	~CPU();

	void initialize();
	void shutDown();
	void frame();
	int getCPUPercentage();

private:
	bool mCanReadCPU;
	HQUERY mQueryHandle;
	HCOUNTER mCounterHandle;
	unsigned long mLastSampleTime;
	long mCpuUsage;
};