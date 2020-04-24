#include "Graphics.h"

bool Graphics::initialize( const int screenWidth, const int screenHeight, HWND hWnd )
{
	mD3D = std::make_unique<D3D>();
	if ( nullptr == mD3D )
	{
		return false;
	}

	if ( false == mD3D->initialize(screenWidth, screenHeight,
								   VSYNC_ENABLE, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR) )
	{
		MessageBox( hWnd, L"Couldn't initialize Direct3D", L"Error", MB_OK );
		return false;
	}

	mCamera = std::make_unique<Camera>();
	if ( nullptr == mCamera )
	{
		return false;
	}
	mCamera->setPosition( 0.f, 0.f, -5.f );

	mModel = std::make_unique<Model>();
	if ( nullptr == mModel )
	{
		return false;
	}

	char textureFileName[] = "data/Diffuse_2K.tga";
	char modelFileName[] = "data/Moon 2K.fbx";
	if ( false == mModel->initialize(mD3D->getDevice(), mD3D->getDeviceContext(), textureFileName, modelFileName) )
	{
		MessageBox( hWnd, L"Could not initialize the model object", L"Error", MB_OK );
		return false;
	}

	mLightShader = std::make_unique<LightShader>();
	if ( nullptr == mLightShader )
	{
		return false;
	}
	
	if ( false == mLightShader->initialize(mD3D->getDevice(), hWnd) )
	{
		MessageBox( hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK );
		return false;
	}

	mLight = std::make_unique<Light>();
	if ( nullptr == mLight )
	{
		return false;
	}

	mLight->setAmbientColor( 0.1f, 0.1f, 0.1f, 1.f );
	mLight->setDiffuseColor( 1.f, 1.f, 1.f, 1.f );
	mLight->setPosition( 0.f, 0.f, -10.f );
	mLight->setSpecularColor( 1.f, 1.f, 1.f, 1.f );
	mLight->setSpecularPower( 5.f );
	
	return true;
}

void Graphics::shutDown( )
{
	if ( nullptr != mD3D )
	{
		mD3D->shutDown( );
	}
	if ( nullptr != mLightShader )
	{
		mLightShader->shutDown( );
	}
	if ( nullptr != mModel )
	{
		mModel->shutDown( );
	}
	//if ( nullptr != mCamera )
	//{ }
}

bool Graphics::frame()
{
	static float Rotation = 0.f;
	Rotation += (float)DirectX::XM_PI*0.04f;
	if ( Rotation > 360.f )
	{
		Rotation -= 360.f;
	}

	if ( false == render(Rotation) )
	{
		return false;
	}
	return true;
}

bool Graphics::render(float rotation)
{
	mD3D->beginScene( 0.f, 0.f, 0.f, 1.f );

	mCamera->render( );

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	mD3D->getWorldMatrix( worldMatrix );
	mCamera->getViewMatrix( viewMatrix );
	mD3D->getProjectionMatrix( projectionMatrix );

	rotation = DirectX::XMConvertToRadians(rotation);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
	worldMatrix = worldMatrix*rotationMatrix;

	mModel->render( mD3D->getDeviceContext() );

	if ( false == mLightShader->render(mD3D->getDeviceContext(),
										mModel->getIndexCount(),
										worldMatrix,
										viewMatrix,
										projectionMatrix,
										mModel->getTexture(),
										mLight->getPosition(),
										mLight->getDiffuseColor(),
									   mLight->getAmbientColor(),
									   mCamera->getPosition(),
									   mLight->getSpecularColor(), mLight->getSpecularPower()) )
	{
		return false;
	}

	mD3D->endScene( );

	return true;
}
