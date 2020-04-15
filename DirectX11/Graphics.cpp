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
	mCamera->setPosition( 0.f, 0.f, -1.f );

	mModel = std::make_unique<Model>();
	if ( nullptr == mModel )
	{
		return false;
	}

	if ( false == mModel->initialize(mD3D->getDevice(), mD3D->getDeviceContext(), "data/wood.tga") )
	{
		MessageBox( hWnd, L"Could not initialize the model object", L"Error", MB_OK );
		return false;
	}

	mTextureShader = std::make_unique<TextureShader>();
	if ( nullptr == mTextureShader )
	{
		return false;
	}
	
	if ( false == mTextureShader->initialize(mD3D->getDevice(), hWnd) )
	{
		MessageBox( hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK );
		return false;
	}
	
	return true;
}

void Graphics::shutDown( )
{
	if ( nullptr != mD3D )
	{
		mD3D->shutDown( );
	}
	if ( nullptr != mTextureShader )
	{
		mTextureShader->shutDown( );
	}
	if ( nullptr != mModel )
	{
		mModel->shutDown( );
	}
	//if ( nullptr != mCamera )
	//{ }
}

bool Graphics::render()
{
	mD3D->beginScene( 0.f, 0.f, 0.f, 1.f );

	mCamera->render( );

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	mD3D->getWorldMatrix( worldMatrix );
	mCamera->getViewMatrix( viewMatrix );
	mD3D->getProjectionMatrix( projectionMatrix );

	mModel->render( mD3D->getDeviceContext() );

	if ( false == mTextureShader->render(mD3D->getDeviceContext(),
										 mModel->getIndexCount(),
										 worldMatrix,
										 viewMatrix,
										 projectionMatrix,
										 mModel->getTexture()) )
	{
		return false;
	}

	mD3D->endScene( );

	return true;
}
