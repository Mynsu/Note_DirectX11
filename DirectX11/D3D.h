#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class D3D
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}

	D3D();
	D3D(const D3D&);
	~D3D();

	bool initialize(int, int, bool, HWND, bool, float, float);
	void shutDown();

	void beginScene(float, float, float, float);
	void endScene();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void getProjectionMatrix(XMMATRIX&);
	void getWorldMatrix(XMMATRIX&);
	void getOrthoMatrix(XMMATRIX&);

	void turnZBufferOn();
	void turnZBufferOff();

	void turnOnAlphaBlending();
	void turnOffAlphaBlending();

private:
	bool mVsyncEnabled;

	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;

	XMMATRIX mProjectionMatrix;
	XMMATRIX mWorldMatrix;
	XMMATRIX mOrthoMatrix;

	ID3D11DepthStencilState* mDepthDisabledStencilState;
	ID3D11BlendState* mAlphaEnableBlendingState;
	ID3D11BlendState* mAlphaDisableBlendingState;
};
