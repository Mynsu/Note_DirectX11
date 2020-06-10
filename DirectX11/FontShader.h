#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class FontShader
{
public:
	FontShader( )
		: mVertexShader( nullptr ), mPixelShader( nullptr ), mLayout( nullptr ), mMatrixBuffer( nullptr ), mPixelBuffer( nullptr )
	{}
	FontShader( const FontShader& ) = delete;
	~FontShader( ) = default;
	bool initialize( ID3D11Device* device, HWND hWnd )
	{
		WCHAR vs[] = L"font.vs";
		WCHAR ps[] = L"font.ps";
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
				ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 pixelColor )
	{
		if ( false == setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor) )
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
	struct PixelBufferType
	{
		DirectX::XMFLOAT4 pixelColor;
	};
	bool initializeShader( ID3D11Device* device, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName );
	void shutDownShader( );
	void outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFileName );
	bool setShaderParameters( ID3D11DeviceContext* deviceContext,
							 DirectX::XMMATRIX& worldMatrix,
							 DirectX::XMMATRIX& viewMatrix,
							 DirectX::XMMATRIX& projectionMatrix,
							 ID3D11ShaderResourceView* texture,
							 DirectX::XMFLOAT4 pixelColor);
	void renderShader( ID3D11DeviceContext* deviceContext, const int indexCount );

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	ID3D11SamplerState* mSampleState;
	ID3D11Buffer* mPixelBuffer;
};