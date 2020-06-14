#pragma once

#include <d3d11.h>
#include <directxmath.h>
//#include <d3dx11async.h>
#include <fstream>
using namespace DirectX;

class FontShader
{
public:
	FontShader();
	FontShader(const FontShader&);
	~FontShader();

	bool initialize(ID3D11Device*, HWND);
	void shutDown();

	bool render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

	bool initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void shutDownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool setShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	void renderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mConstantBuffer;
	ID3D11SamplerState* mSampleState;
	ID3D11Buffer* mPixelBuffer;
};
