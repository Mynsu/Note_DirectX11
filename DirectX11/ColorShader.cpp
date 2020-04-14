#include "ColorShader.h"
#include <d3dcompiler.h>
#include <fstream>

bool ColorShader::initializeShader( ID3D11Device* device, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName )
{
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	HRESULT result = D3DCompileFromFile( vsFileName, NULL, NULL,
										"ColorVertexShader", "vs_5_0",
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
								"ColorPixelShader", "ps_5_0",
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

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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

	return true;
}

void ColorShader::shutDownShader( )
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
		mPixelShader->Release( );
		mPixelShader = nullptr;
	}
}

void ColorShader::outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFileName )
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

bool ColorShader::setShaderParameters( ID3D11DeviceContext* deviceContext,
									  DirectX::XMMATRIX& worldMatrix,
									  DirectX::XMMATRIX& viewMatrix,
									  DirectX::XMMATRIX& projectionMatrix )
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

	unsigned int bufferNumber = 0;
	deviceContext->VSSetConstantBuffers( bufferNumber, 1, &mMatrixBuffer );

	return true;
}

void ColorShader::renderShader( ID3D11DeviceContext* deviceContext, const int indexCount )
{
	deviceContext->IASetInputLayout( mLayout );
	deviceContext->VSSetShader( mVertexShader, NULL, 0 );
	deviceContext->PSSetShader( mPixelShader, NULL, 0 );
	deviceContext->DrawIndexed( indexCount, 0, 0 );
}
