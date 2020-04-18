#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class TextureShader
{
public:
	TextureShader( )
		: mVertexShader( nullptr ), mPixelShader( nullptr ), mLayout( nullptr ), mMatrixBuffer( nullptr ),
		mSampleState( nullptr )
	{}
	TextureShader( const TextureShader& ) = delete;
	~TextureShader( ) = default;
	bool initialize( ID3D11Device* device, HWND hWnd )
	{
		WCHAR vs[] = L"texture.vs";
		WCHAR ps[] = L"texture.ps";
		if ( false == initializeShader(device, hWnd, vs, ps) )
		{
			return false;
		}

		return true;
	}
	void shutDown()
	{
		shutDownShader();
	}
	bool render( ID3D11DeviceContext* deviceContext, int indexCount,
				DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix,
				ID3D11ShaderResourceView* texture )
	{
		if ( false == setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture) )
		{
			return false;
		}
		renderShader(deviceContext, indexCount);

		return true;
	}
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	bool initializeShader( ID3D11Device* device, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName );
	void shutDownShader( );
	void outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFileName );
	bool setShaderParameters( ID3D11DeviceContext* deviceContext,
							 DirectX::XMMATRIX& worldMatrix,
							 DirectX::XMMATRIX& viewMatrix,
							 DirectX::XMMATRIX& projectionMatrix,
							 ID3D11ShaderResourceView* texture);
	void renderShader( ID3D11DeviceContext* deviceContext, const int indexCount );

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	ID3D11SamplerState* mSampleState;
};