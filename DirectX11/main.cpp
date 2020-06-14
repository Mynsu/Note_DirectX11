#include "system.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	System* system;
	bool result;


	// Create the system object.
	system = new System;
	if(!system)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = system->initialize();
	if(result)
	{
		system->run();
	}

	// Shutdown and release the system object.
	system->shutDown();
	delete system;
	system = 0;

	return 0;
}