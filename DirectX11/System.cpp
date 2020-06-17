#include "system.h"


System::System()
{
	mInput = 0;
	mGraphics = 0;
	m_Position = 0;
	mFPS = 0;
	mCPU = 0;
	mTimer = 0;
}


System::System(const System& other)
{
}


System::~System()
{
}


bool System::initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	initializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	mInput = new Input;
	if(!mInput)
	{
		return false;
	}

	// Initialize the input object.
	result = mInput->initialize(mhInstance, mhWnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(mhWnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	mGraphics = new Graphics;
	if(!mGraphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = mGraphics->initialize(screenWidth, screenHeight, mhWnd);
	if(!result)
	{
		return false;
	}

	// Create the position object.
	m_Position = new Position;
	if(!m_Position)
	{
		return false;
	}

	// Create the fps object.
	mFPS = new FPS;
	if(!mFPS)
	{
		return false;
	}

	// Initialize the fps object.
	mFPS->initialize();

	// Create the cpu object.
	mCPU = new CPU;
	if(!mCPU)
	{
		return false;
	}

	// Initialize the cpu object.
	mCPU->initialize();

	// Create the timer object.
	mTimer = new Timer;
	if(!mTimer)
	{
		return false;
	}

	// Initialize the timer object.
	result = mTimer->initialize();
	if(!result)
	{
		MessageBox(mhWnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void System::shutDown()
{
	// Release the timer object.
	if(mTimer)
	{
		delete mTimer;
		mTimer = 0;
	}

	// Release the cpu object.
	if(mCPU)
	{
		mCPU->shutDown();
		delete mCPU;
		mCPU = 0;
	}

	// Release the fps object.
	if(mFPS)
	{
		delete mFPS;
		mFPS = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the graphics object.
	if(mGraphics)
	{
		mGraphics->shutDown();
		delete mGraphics;
		mGraphics = 0;
	}

	// Release the input object.
	if(mInput)
	{
		mInput->shutDown();
		delete mInput;
		mInput = 0;
	}

	// Shutdown the window.
	shutDownWindows();

	return;
}


void System::run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.  If frame processing fails then exit.
			result = frame();
			if(!result)
			{
				MessageBox(mhWnd, L"frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if(mInput->isEscapePressed() == true)
		{
			done = true;
		}
	}

	return;
}


bool System::frame()
{
	bool keyDown, result;
	float rotationY;

	// Update the system stats.
	mTimer->frame();
	///mFPS->frame();
	///mCPU->frame();

	// Do the input frame processing.
	result = mInput->frame();
	if(!result)
	{
		return false;
	}

	/// Get the location of the mouse from the input object,
	///m_Input->GetMouseLocation(mouseX, mouseY);

	/// Do the frame processing for the graphics object.
	///result = m_Graphics->Frame(mouseX, mouseY);
	///if(!result)
	///{
	///	return false;
	///}

	// Set the frame time for calculating the updated position.
	m_Position->setFrameTime(mTimer->getTime());

	// Check if the left or right arrow key has been pressed, if so rotate the camera accordingly.
	keyDown = mInput->isLeftArrowPressed();
	m_Position->turnLeft(keyDown);

	keyDown = mInput->isRightArrowPressed();
	m_Position->turnRight(keyDown);

	// Get the current view point rotation.
	m_Position->getRotation(rotationY);

	// Do the frame processing for the graphics object.
	result = mGraphics->frame(rotationY);
	if(!result)
	{
		return false;
	}

	// Finally render the graphics to the screen.
	result = mGraphics->render();
	if(!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK System::messageHandler(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hWnd, umsg, wparam, lparam);
}


void System::initializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	mhInstance = GetModuleHandle(NULL);

	// Give the application a name.
	mApplicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = mApplicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	mhWnd = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName, 
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, screenWidth, screenHeight, NULL, NULL, mhInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(mhWnd, SW_SHOW);
	SetForegroundWindow(mhWnd);
	SetFocus(mhWnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void System::shutDownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(mhWnd);
	mhWnd = NULL;

	// Remove the application instance.
	UnregisterClass(mApplicationName, mhInstance);
	mhInstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->messageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}