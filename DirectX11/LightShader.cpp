#include "LightShader.h"
#include <d3dcompiler.h>
#include <fstream>

bool LightShader::initializeShader( ID3D11Device* device, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName )
{
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	HRESULT result = D3DCompileFromFile( vsFileName, NULL, NULL,
										"LightVertexShader", "vs_5_0",
										D3D10_SHADER_ENABLE_STRICTNESS, 0,
										&vertexShaderBuffer, &errorMessage );
	if ( FAILED(result) )
	{
		if ( nullptr != errorMessage )
		{
			outputShaderErrorMessage( errorMessage, hWnd, vsFileName );
		}
		else
		{
			MessageBox( hWnd, vsFileName, L"Missing Shader File", MB_OK );
		}
		return false;
	}

	result = D3DCompileFromFile( psFileName, NULL, NULL,
								"LightPixelShader", "ps_5_0",
								D3D10_SHADER_ENABLE_STRICTNESS, 0,
								&pixelShaderBuffer, &errorMessage );
	if ( FAILED(result) )
	{
		if ( nullptr != errorMessage )
		{
			outputShaderErrorMessage( errorMessage, hWnd, vsFileName );
		}
		else
		{
			MessageBox( hWnd, vsFileName, L"Missing Shader File", MB_OK );
		}

		return false;
	}

	result = device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(),
										vertexShaderBuffer->GetBufferSize(),
										NULL,
										&mVertexShader );
	if ( FAILED(result) )
	{
		return false;
	}

	result = device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(),
									   pixelShaderBuffer->GetBufferSize(),
									   NULL,
									   &mPixelShader );
	if ( FAILED(result) )
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	result = device->CreateInputLayout(polygonLayout, numElements,
									   vertexShaderBuffer->GetBufferPointer(),
									   vertexShaderBuffer->GetBufferSize(),
									   &mLayout );
	if ( FAILED(result) )
	{
		return false;
	}

	vertexShaderBuffer->Release( );
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer->Release( );
	pixelShaderBuffer = nullptr;

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	result = device->CreateBuffer( &matrixBufferDesc, NULL, &mMatrixBuffer );
	if ( FAILED(result) )
	{
		return false;	
	}

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 가장 중요.
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// 아래는 기본값들.
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = device->CreateSamplerState(&samplerDesc, &mSampleState);
	if ( FAILED(result) )
	{
		return false;
	}

	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	result = device->CreateBuffer(&lightBufferDesc, NULL, &mLightBuffer);
	if ( FAILED(result) )
	{
		return false;
	}

	return true;
}

void LightShader::shutDownShader( )
{
	if ( nullptr != mMatrixBuffer )
	{
		mMatrixBuffer->Release( );
		mMatrixBuffer = nullptr;
	}
	if ( nullptr != mLayout )
	{
		mLayout->Release( );
		mPixelShader = nullptr;
	}
	if ( nullptr != mVertexShader )
	{
		mVertexShader->Release( );
		mVertexShader = nullptr;
	}
	if ( nullptr != mPixelShader )
	{
		mPixelShader->Release();
		mPixelShader = nullptr;
	}
	if ( nullptr != mSampleState )
	{
		mSampleState->Release();
		mSampleState = nullptr;
	}
	if ( nullptr != mLightBuffer )
	{
		mLightBuffer->Release();
		mLightBuffer = nullptr;
	}
}

void LightShader::outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFileName )
{
	char* compileErrors = (char*)(errorMessage->GetBufferPointer());
	unsigned long long bufferSize = errorMessage->GetBufferSize();
	std::ofstream fout;
	fout.open( "shader-error.txt" );
	for ( unsigned long long index = 0; index < bufferSize; ++index )
	{
		fout << compileErrors[index];
	}
	fout.close( );
	errorMessage->Release( );
	errorMessage = nullptr;
	MessageBox( hWnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFileName, MB_OK);
}

bool LightShader::setShaderParameters( ID3D11DeviceContext* deviceContext,
										DirectX::XMMATRIX& worldMatrix,
										DirectX::XMMATRIX& viewMatrix,
										DirectX::XMMATRIX& projectionMatrix,
										ID3D11ShaderResourceView* texture,
										DirectX::XMFLOAT3 lightPosition,
										DirectX::XMFLOAT4 diffuseColor,
										DirectX::XMFLOAT4 ambientColor )
{
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if ( FAILED(result) )
	{
		return false;
	}

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	deviceContext->Unmap( mMatrixBuffer, 0) ;

	unsigned int vertexBufferNumber = 0;
	deviceContext->VSSetConstantBuffers( vertexBufferNumber, 1, &mMatrixBuffer );

	deviceContext->PSSetShaderResources(0, 1, &texture);

	result = deviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED(result) )
	{
		return false;
	}
	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightPosition = lightPosition;
	dataPtr2->padding = 0.0f;
	deviceContext->Unmap(mLightBuffer, 0);

	++vertexBufferNumber;
	unsigned int pixelBufferNumber = 0;
	deviceContext->VSSetConstantBuffers( vertexBufferNumber, 1, &mLightBuffer );
	deviceContext->PSSetConstantBuffers( pixelBufferNumber, 1, &mLightBuffer );

	return true;
}

void LightShader::renderShader( ID3D11DeviceContext* deviceContext, const int indexCount )
{
	deviceContext->IASetInputLayout( mLayout );
	deviceContext->VSSetShader( mVertexShader, NULL, 0 );
	deviceContext->PSSetShader( mPixelShader, NULL, 0 );
	deviceContext->PSSetSamplers( 0, 1, &mSampleState );
	deviceContext->DrawIndexed( indexCount, 0, 0 );
}