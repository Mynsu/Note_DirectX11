#pragma once

#include "d3d.h"
#include "camera.h"
#include "text.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool initialize(int, int, HWND);
	void shutDown();
	void frame();
	bool render();

private:
	D3D* mD3D;
	Camera* mCamera;
	Text* mText;
};
