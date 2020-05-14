#include "System.h"

bool System::initialize()
{
	int screenWidth = 0;
	int screenHeight = 0;
	initializeWindow( &screenWidth, &screenHeight );

	mInput = std::make_unique<Input>();
	if ( nullptr == mInput )
	{
		return false;
	}

	mGraphics = std::make_unique<Graphics>();
	if ( nullptr == mGraphics )
	{
		return false;
	}
	if ( false == mGraphics->initialize(screenWidth, screenHeight, mhWnd) ) 
	{
		return false;
	}
	//std::string fileName("Script.lua");
	//mGraphics->loadFromScript( fileName );
	
	return true;
}

void System::run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while ( true )
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if ( WM_QUIT == msg.message )
		{
			break;
		}
		else
		{
			if ( false == frame() )
			{
				break;
			}
		}
	}
}

LRESULT System::MessageHandler( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
	LRESULT retVal = 0;
	switch ( umsg )
	{
		case WM_KEYDOWN:
			mInput->keyDown((unsigned)wparam);
			break;
		case WM_KEYUP:
			mInput->keyUp((unsigned)wparam);
			break;
		default:
			retVal = DefWindowProc(hwnd, umsg, wparam, lparam);
			break;
	}

	return retVal;
}

bool System::frame()
{
	if ( true == mInput->isKeyDown(VK_ESCAPE) )
	{
		return false;
	}
	//if ( true == mInput->isKeyDown('F') )
	//{
	//	//std::string fileName("Script.lua");
	//	//mGraphics->loadFromScript( fileName );
	//}
	if ( false == mGraphics->frame() )
	{
		return false;
	}

	return true;
}

void System::initializeWindow( int* screenWidth, int* screenHeight )
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	AppHandle = this;
	mhInstance = GetModuleHandle(NULL);
	mAppName = L"Engine";
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mAppName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx( &wc );

	*screenWidth = GetSystemMetrics(SM_CXSCREEN);
	*screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if ( true == FULL_SCREEN )
	{
		ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (DWORD)*screenWidth;
		dmScreenSettings.dmPelsHeight = (DWORD)*screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN );
		posX = posY = 0;
	}
	else
	{
		*screenWidth = 800;
		*screenHeight = 600;
		posX = (GetSystemMetrics(SM_CXSCREEN) - *screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - *screenHeight) / 2;
	}

	mhWnd = CreateWindowEx( WS_EX_APPWINDOW, mAppName, mAppName,
						   WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						   posX, posY, *screenWidth, *screenHeight,
						   NULL, NULL, mhInstance, NULL );
	ShowWindow( mhWnd, SW_SHOW );
	SetForegroundWindow( mhWnd );
	SetFocus( mhWnd );
	ShowCursor( false );
}

void System::shutdownWindow()
{
	ShowCursor( true );
	if ( true == FULL_SCREEN )
	{
		ChangeDisplaySettings( NULL, 0 );
	}
	DestroyWindow(mhWnd);
	mhWnd = nullptr;
	UnregisterClass(mAppName, mhInstance);
	AppHandle = nullptr;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
	LRESULT retVal = 0;
	switch ( umsg )
	{
		case WM_DESTROY:
			[[ fallthrough ]];
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			retVal = AppHandle->MessageHandler(hWnd, umsg, wparam, lparam);
			break;
	}

	return retVal;
}
