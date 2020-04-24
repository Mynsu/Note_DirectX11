#pragma once
#include <memory>
#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "LightShader.h"
#include "Light.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLE = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics( ) = default;
	Graphics( const Graphics& ) = delete;
	~Graphics( ) = default;

	bool initialize( const int screenWidth, const int screenHeight, HWND hWnd );
	void shutDown( );
	bool frame( );
private:
	bool render( float rotation, DirectX::XMFLOAT3 position );

	std::unique_ptr<D3D> mD3D;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<Model> mModel;
	std::unique_ptr<LightShader> mLightShader;
	std::unique_ptr<Light> mLight;
};
