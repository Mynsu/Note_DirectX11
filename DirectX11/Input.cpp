#include "input.h"


Input::Input()
{
}


Input::Input(const Input& other)
{
}


Input::~Input()
{
}


void Input::initialize()
{
	int i;


	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		mKeys[i] = false;
	}

	return;
}


void Input::keyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	mKeys[input] = true;
	return;
}


void Input::keyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	mKeys[input] = false;
	return;
}


bool Input::isKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return mKeys[key];
}