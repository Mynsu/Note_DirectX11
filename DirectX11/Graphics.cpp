#include "Graphics.h"
#include "ScriptLoader.h"

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

	//mModel = std::make_unique<Model>();
	//if ( nullptr == mModel )
	//{
	//	return false;
	//}

	//char textureFileName[] = "data/Diffuse_2K.tga";
	//char modelFileName[] = "data/Moon 2K.fbx";
	//if ( false == mModel->initialize(mD3D->getDevice(), mD3D->getDeviceContext(), textureFileName, modelFileName) )
	//{
	//	MessageBox( hWnd, L"Could not initialize the model object", L"Error", MB_OK );
	//	return false;
	//}

	//mLightShader = std::make_unique<LightShader>();
	//if ( nullptr == mLightShader )
	//{
	//	return false;
	//}
	//
	//if ( false == mLightShader->initialize(mD3D->getDevice(), hWnd) )
	//{
	//	MessageBox( hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK );
	//	return false;
	//}

	//mLight = std::make_unique<Light>();
	//if ( nullptr == mLight )
	//{
	//	return false;
	//}

	//mLight->setAmbientColor( 0.1f, 0.1f, 0.1f, 1.f );
	//mLight->setDiffuseColor( 1.f, 1.f, 1.f, 1.f );
	////mLight->setPosition( 0.f, 1.f, -3.f );
	//mLight->setPosition( 0.f, 0.f, -10.f );
	//mLight->setSpecularColor( 1.f, 1.f, 1.f, 1.f );
	//mLight->setSpecularPower( 5.f );

	/*mBitmap = std::make_unique<Bitmap>();
	if ( nullptr == mBitmap )
	{
		return false;
	}
	std::string uiPath( "data/wood.tga" );
	bool result = mBitmap->initialize(mD3D->getDevice(), mD3D->getDeviceContext(),
								 screenWidth, screenHeight, uiPath.data(), 300, 300);
	if ( false == result )
	{
		MessageBox(hWnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	mTextureShader = std::make_unique<TextureShader>();
	if ( nullptr == mTextureShader )
	{
		return false;
	}
	result = mTextureShader->initialize(mD3D->getDevice(), hWnd);
	if ( false == result )
	{
		MessageBox(hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	mTexture = std::make_unique<Texture>();
	if ( nullptr == mTexture )
	{
		return false;
	}*/

	mCamera->setPosition(0.0f, 0.0f, -1.0f);
	mCamera->render();
	DirectX::XMMATRIX baseViewMatrix;
	mCamera->getViewMatrix(baseViewMatrix);
	mText = std::make_unique<Text>();
	if(!mText)
	{
		return false;
	}
	bool result = mText->initialize(mD3D->getDevice(),
									mD3D->getDeviceContext(),
									hWnd,
									screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	
	return true;
}

//void Graphics::loadFromScript(std::string& fileName)
//{
//	std::vector<std::string> vars = 
//		{
//			"CameraX", "CameraY", "CameraZ",
//			"LightX", "LightY", "LightZ"
//		};
//	auto result = ::util::script::LoadFromScript(fileName, vars);
//
//	DirectX::XMFLOAT3 cameraPos;
//	if ( auto it = result.find(vars[0]);
//		result.cend() != it )
//	{
//		cameraPos.x = std::get<float>(it->second);
//	}
//	if ( auto it = result.find(vars[1]);
//		result.cend() != it )
//	{
//		cameraPos.y = std::get<float>(it->second);
//	}
//	if ( auto it = result.find(vars[2]);
//		result.cend() != it )
//	{
//		cameraPos.z = std::get<float>(it->second);
//	}
//	DirectX::XMFLOAT3 lightPos;
//	if ( auto it = result.find(vars[3]);
//		result.cend() != it )
//	{
//		lightPos.x = std::get<float>(it->second);
//	}
//	if ( auto it = result.find(vars[4]);
//		result.cend() != it )
//	{
//		lightPos.y = std::get<float>(it->second);
//	}
//	if ( auto it = result.find(vars[5]);
//		result.cend() != it )
//	{
//		lightPos.z = std::get<float>(it->second);
//	}
//
//	mCamera->setPosition( cameraPos.x, cameraPos.y, cameraPos.z );
//	mLight->setPosition( lightPos.x, lightPos.y, lightPos.z );
//}

void Graphics::shutDown( )
{
	if(mText)
	{
		mText->shutDown();
	}
	if ( nullptr != mD3D )
	{
		mD3D->shutDown( );
	}
	/*if ( nullptr != mLightShader )
	{
		mLightShader->shutDown( );
	}
	if ( nullptr != mModel )
	{
		mModel->shutDown( );
	}*/
	//if ( nullptr != mCamera )
	//{ }
	/*if ( nullptr != mBitmap )
	{
		mBitmap->shutDown();
	}*/
}

bool Graphics::frame()
{
	static float Rotation = 0.f;
	Rotation += (float)DirectX::XM_PI*0.04f;
	if ( Rotation > 360.f )
	{
		Rotation -= 360.f;
	}
	//
	//static DirectX::XMFLOAT3 Position(0.f, 0.f, 0.f);
	//static float DeltaZ = 0.01f;
	/*if ( 1.f < Position.z || Position.z < -1.f )
	{
		DeltaZ *= -1.f;
	}
	Position.z += DeltaZ;*/
	//

	//if ( false == render(Rotation, Position) )
	if ( false == render(Rotation) )
	{
		return false;
	}
	return true;
}

//bool Graphics::render(float rotation, DirectX::XMFLOAT3 position)
bool Graphics::render(float rotation)
{
	mD3D->beginScene( 0.f, 0.f, 0.f, 1.f );

	mCamera->render( );

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthogonalMatrix;
	mD3D->getWorldMatrix( worldMatrix );
	mCamera->getViewMatrix( viewMatrix );
	mD3D->getProjectionMatrix( projectionMatrix );
	mD3D->getOrthogonalMatrix( orthogonalMatrix );

	mD3D->turnZBufferOff();

	/*bool result = mBitmap->render(mD3D->getDeviceContext(), 100, 100);
	if ( false == result )
	{
		return false;
	}

	result = mTextureShader->render(mD3D->getDeviceContext(), mBitmap->getIndexCount(),
									worldMatrix, viewMatrix, orthogonalMatrix, mBitmap->getTexture());
	if ( false == result )
	{
		return false;
	}*/

	mD3D->turnOnAlphaBlending();
	bool result = mText->render(mD3D->getDeviceContext(), worldMatrix, orthogonalMatrix);
	if(!result)
	{
		return false;
	}
	mD3D->turnOffAlphaBlending();

	mD3D->turnZBufferOn();

	//
	//DirectX::XMMatrixTranslationFromVector(position);
	//DirectX::XMMATRIX traslationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//worldMatrix += traslationMatrix;
	//

	/*rotation = DirectX::XMConvertToRadians(rotation);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
	worldMatrix = worldMatrix*rotationMatrix;*/

	/*mModel->render( mD3D->getDeviceContext() );

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
	}*/

	mD3D->endScene( );

	return true;
}
