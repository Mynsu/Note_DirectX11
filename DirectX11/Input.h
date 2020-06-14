#pragma once
#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>
#include <vector>

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	bool initialize(HINSTANCE, HWND, int, int);
	void shutDown();

	bool frame();

	bool isEscapePressed();
	void getMouseLocation(int&, int&);
	unsigned char getKeyPressed();

private:
	bool readKeyboard();
	bool readMouse();
	void processInput();

	IDirectInput8* mDirectInput;
	IDirectInputDevice8* mKeyboard;
	IDirectInputDevice8* mMouse;

	unsigned char mKeyboardState[256];
	DIMOUSESTATE mMouseState;
	int mScreenWidth, mScreenHeight;
	int mMouseX, mMouseY;
};
