#include "D3D.h"

D3D::D3D()
	: mIsVsyncEnabled( false ), mVideoCardMemory( 0 ),
	mSwapChain( nullptr ), mDevice( nullptr ), mDeviceContext( nullptr ),
	mRenderTargetView( nullptr ),
	mDepthStencilBuffer( nullptr ), mDepthStencilState( nullptr ), mDepthStencilView( nullptr ),
	mRasterizerState( nullptr )
{
	ZeroMemory( mVideoCardDescription, sizeof(mVideoCardDescription) );
}

bool D3D::initialize( const int screenWidth, const int screenHeight,
					 const bool isVsyncEnabled, HWND hWnd, const bool isFullScreen,
					 const float screenDepth, const float screenNear )
{
	mIsVsyncEnabled = isVsyncEnabled;
	IDXGIFactory* factory = nullptr;
	HRESULT result = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&factory );
	if ( FAILED(result) )
	{
		return false;
	}

	IDXGIAdapter* adapter = nullptr;
	result = factory->EnumAdapters(0, &adapter);
	if ( FAILED(result) )
	{
		return false;
	}

	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if ( FAILED(result) )
	{
		return false;
	}

	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
											   DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if ( FAILED(result) )
	{
		return false;
	}

	DXGI_MODE_DESC* displayModeList = nullptr;
	displayModeList = new DXGI_MODE_DESC[numModes];
	if ( nullptr == displayModeList )
	{
		return false;
	}

	unsigned int numerator = 0;
	unsigned int denominator = 0;
	for ( unsigned int index = 0; index < numModes; ++index )
	{
		if ( (unsigned int)screenWidth == displayModeList[index].Width && 
			(unsigned int)screenHeight == displayModeList[index].Height	)
		{
			numerator = displayModeList[index].RefreshRate.Numerator;
			denominator = displayModeList[index].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if ( FAILED(result) )
	{
		return false;
	}

	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024	);

	size_t stringLength = 0;
	if ( 0 != wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128) )
	{
		return false;
	}
	
	delete[] displayModeList;
	displayModeList = nullptr;
	adapterOutput->Release();
	adapterOutput = nullptr;
	adapter->Release();
	adapter = nullptr;
	factory->Release();
	adapter = nullptr;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if ( true == mIsVsyncEnabled )
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	if ( true == isFullScreen )
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
										   &featureLevel, 1,
										   D3D11_SDK_VERSION,
										   &swapChainDesc, &mSwapChain,
										   &mDevice, NULL, &mDeviceContext );
	if ( FAILED(result) )
	{
		return false;
	}

	ID3D11Texture2D* backBufferPtr = nullptr;
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if ( FAILED(result) )
	{
		return false;
	}

	result = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
	if ( FAILED(result) )
	{
		return false;
	}
	backBufferPtr->Release();
	backBufferPtr = nullptr;


	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	if ( FAILED(result) )
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof(depthStencilDesc) );
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if ( FAILED(result) )
	{
		return false;	
	}

	mDeviceContext->OMSetDepthStencilState( mDepthStencilState, 1 );

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory( &depthStencilViewDesc, sizeof(depthStencilViewDesc) );
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if ( FAILED(result) )
	{
		return false;
	}

	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.f;

	result = mDevice->CreateRasterizerState(&rasterizerDesc, &mRasterizerState);
	if ( FAILED(result) )
	{
		return false;
	}

	mDeviceContext->RSSetState( mRasterizerState );

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	mDeviceContext->RSSetViewports( 1, &viewport );

	float fieldOfView = 3.141592654f / 4.f;
	float screenAspect = (float)screenWidth / (float)screenHeight;
	mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	mWorldMatrix = DirectX::XMMatrixIdentity();
	mOrthogonalMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3D::shutDown( )
{
	if ( nullptr !=	mSwapChain )
	{
		mSwapChain->SetFullscreenState( false, NULL );
	}

	if ( nullptr != mRasterizerState )
	{
		mRasterizerState->Release( );
		mRasterizerState = nullptr;
	}

	if ( nullptr != mDepthStencilView )
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}

	if ( nullptr != mDepthStencilState )
	{
		mDepthStencilState->Release();
		mDepthStencilState = nullptr;
	}

	if ( nullptr != mDepthStencilBuffer )
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = nullptr;
	}

	if ( nullptr != mRenderTargetView )
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}

	if ( nullptr != mDeviceContext )
	{
		mDeviceContext->Release();
		mDeviceContext = nullptr;
	}

	if ( nullptr != mDevice )
	{
		mDevice->Release( );
		mSwapChain = nullptr;
	}
}

void D3D::beginScene( const float red, const float green, const float blue, const float alpha )
{
	float color[4] = { red, green, blue, alpha };

	mDeviceContext->ClearRenderTargetView( mRenderTargetView, color );
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0 );
}

void D3D::endScene( )
{
	if ( true == mIsVsyncEnabled )
	{
		mSwapChain->Present(1, 0);
	}
	else
	{
		mSwapChain->Present(0, 0);
	}
}


