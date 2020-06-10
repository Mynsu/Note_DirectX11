#pragma once
#include <memory>
#include "D3D.h"
#include "Camera.h"
//#include "Model.h"
//#include "Bitmap.h"
//#include "LightShader.h"
//#include "Light.h"
//#include "TextureShader.h"
//#include "Texture.h"
#include "Text.h"

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
	//void loadFromScript( std::string& fileName );
	void shutDown( );
	bool frame( );
private:
	//bool render( float rotation, DirectX::XMFLOAT3 position );
	bool render( float rotation );

	std::unique_ptr<D3D> mD3D;
	std::unique_ptr<Camera> mCamera;
	//std::unique_ptr<Model> mModel;
	//std::unique_ptr<LightShader> mLightShader;
	//std::unique_ptr<Light> mLight;
	//std::unique_ptr<Bitmap> mBitmap;
	//std::unique_ptr<TextureShader> mTextureShader;
	//std::unique_ptr<Texture> mTexture;
	std::unique_ptr<Text> mText;
};
