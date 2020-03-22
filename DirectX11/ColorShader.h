#pragma once
#include <fstream>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

class ColorShader
{
public:
	ColorShader( )
		: mVertexShader( nullptr ), mPixelShader( nullptr ), mLayout( nullptr ), mMatrixBuffer( nullptr )
	{}
	ColorShader( const ColorShader& ) = delete;
	~ColorShader( ) = default;
	bool initialize( ID3D11Device* device, HWND hWnd )
	{
		WCHAR vs[] = L"color.vs";
		WCHAR ps[] = L"color.ps";
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
				DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix )
	{
		if ( false == setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix) )
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
	bool setShaderParameters( ID3D11DeviceContext*, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX& );
	void renderShader( ID3D11DeviceContext* deviceContext, const int indexCount );
	
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
};