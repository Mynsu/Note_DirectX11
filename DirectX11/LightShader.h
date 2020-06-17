#pragma once
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class LightShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};

public:
	LightShader();
	LightShader(const LightShader&);
	~LightShader();

	bool initialize(ID3D11Device*, HWND);
	void shutDown();
	bool render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

private:
	bool initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void shutDownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, 
							 XMFLOAT4);
	void renderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11SamplerState* mSampleState;
	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mLightBuffer;
};
