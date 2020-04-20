#pragma once
#pragma comment(lib, "d3d11" )
#pragma comment(lib, "dxgi" )
#pragma comment(lib, "d3dcompiler" )
#pragma comment(lib, "directxtex.lib")
#include <d3d11.h>
#include <DirectXMath.h>

class D3D
{
public:
	D3D();
	D3D( const D3D& ) = delete;
	~D3D() = default;

	bool initialize( const int screenWidth, const int screenHeight,
					const bool isVsyncEnabled, HWND hWindow, const bool isFullScreen,
					const float screenDepth, const float screenNear );
	void shutDown( );
	void beginScene( const float red, const float green, const float blue, const float alpha );
	void endScene( );

	ID3D11Device* getDevice( )
	{
		return mDevice;
	}
	ID3D11DeviceContext* getDeviceContext( )
	{
		return mDeviceContext;
	}
	void getProjectionMatrix( DirectX::XMMATRIX& projectionMatrix )
	{
		projectionMatrix = mProjectionMatrix;
	}
	void getWorldMatrix( DirectX::XMMATRIX& worldMatrix )
	{
		worldMatrix = mWorldMatrix;
	}
	void getOrthogonal( DirectX::XMMATRIX& orthogonalMatrix )
	{
		orthogonalMatrix = mOrthogonalMatrix;
	}
	void getVideoCardInfo( char* cardName, int& memory )
	{
		strcpy_s( cardName, 128, mVideoCardDescription );
		memory = mVideoCardMemory;
	}
private:
	bool mIsVsyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterizerState;
	DirectX::XMMATRIX mProjectionMatrix;
	DirectX::XMMATRIX mWorldMatrix;
	DirectX::XMMATRIX mOrthogonalMatrix;
};
