#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Input.h"
#include "Graphics.h"

class System
{
public:
	System( )
		: mAppName( nullptr ), mhInstance( 0 ), mhWnd( 0 ), mInput( nullptr ), mGraphics( nullptr )
	{ }
	System( const System& ) = delete;
	~System( ) = default;

	bool initialize( );
	void run( );
	void shutDown( )
	{
		if ( nullptr != mGraphics )
		{
			mGraphics->shutDown();
		}
		shutdownWindow();
	}
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool frame( );
	void initializeWindow( int* screenWidth, int* screenHeight );
	void shutdownWindow( );

	LPCWSTR mAppName;
	HINSTANCE mhInstance;
	HWND mhWnd;
	std::unique_ptr<Input> mInput;
	std::unique_ptr<Graphics> mGraphics;
};

static LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

static System* AppHandle = nullptr;