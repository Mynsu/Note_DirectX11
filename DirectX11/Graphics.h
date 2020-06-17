#pragma once

#include "d3d.h"
#include "camera.h"
#include "text.h"
#include "model.h"
#include "lightshader.h"
#include "light.h"
#include "modellist.h"
#include "frustum.h"

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
	///bool Frame(int, int);
	///bool frame(int, int, float);
	bool frame(float);
	bool render();

private:
	D3D* mD3D;
	Camera* mCamera;
	Text* mText;
	Model* mModel;
	LightShader* mLightShader;
	Light* mLight;
	ModelList* mModelList;
	Frustum* mFrustum;
};
