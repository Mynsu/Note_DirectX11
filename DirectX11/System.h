#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "input.h"
#include "graphics.h"

class System
{
public:
	System();
	System(const System&);
	~System();

	bool initialize();
	void shutDown();
	void run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool frame();
	void initializeWindows(int&, int&);
	void shutDownWindows();

private:
	LPCWSTR mApplicationName;
	HINSTANCE mhInstance;
	HWND mhWnd;

	Input* mInput;
	Graphics* mGraphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static System* ApplicationHandle = 0;
