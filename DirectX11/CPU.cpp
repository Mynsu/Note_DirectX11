#include "CPU.h"

CPU::CPU()
{
}


CPU::CPU(const CPU& other)
{
}


CPU::~CPU()
{
}

void CPU::initialize()
{
	PDH_STATUS status;


	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	mCanReadCPU = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &mQueryHandle);
	if(status != ERROR_SUCCESS)
	{
		mCanReadCPU = false;
	}

	// Set query object to poll all cpus in the system.
	status = PdhAddCounter(mQueryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &mCounterHandle);
	if(status != ERROR_SUCCESS)
	{
		mCanReadCPU = false;
	}

	mLastSampleTime = GetTickCount(); 

	mCpuUsage = 0;

	return;
}

void CPU::shutDown()
{
	if(mCanReadCPU)
	{
		PdhCloseQuery(mQueryHandle);
	}

	return;
}

void CPU::frame()
{
	PDH_FMT_COUNTERVALUE value; 

	if(mCanReadCPU)
	{
		if((mLastSampleTime + 1000) < GetTickCount())
		{
			mLastSampleTime = GetTickCount(); 

			PdhCollectQueryData(mQueryHandle);
        
			PdhGetFormattedCounterValue(mCounterHandle, PDH_FMT_LONG, NULL, &value);

			mCpuUsage = value.longValue;
		}
	}

	return;
}

int CPU::getCPUPercentage()
{
	int usage;

	if(mCanReadCPU)
	{
		usage = (int)mCpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;
}