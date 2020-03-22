#include <memory>
#include "System.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, PSTR, int )
{
	std::unique_ptr<System> system( std::make_unique<System>() );
	if ( true == system->initialize() )
	{
		system->run();
	}
	system->shutDown();

	return 0;
}