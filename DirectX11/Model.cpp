#include "Model.h"
#include "Texture.h"

bool Model::initializeBuffers( ID3D11Device* device )
{
	mVertexCount = 3;
	mIndexCount = 3;
	VertexType* vertices = nullptr;
	vertices = new VertexType[mVertexCount];
	if ( nullptr == vertices )
	{
		return false;
	}

	unsigned long* indices = nullptr;
	indices = new unsigned long[mIndexCount];
	if ( nullptr == indices )
	{
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(-1.f, -1.f, 0.f); //Bottom Left
	vertices[0].texture = DirectX::XMFLOAT2(0.f, 1.f);
	vertices[0].normal = DirectX::XMFLOAT3(0.f, 0.f, -1.f);

	vertices[1].position = DirectX::XMFLOAT3(0.f, 1.f, 0.f); //Top Middle
	vertices[1].texture = DirectX::XMFLOAT2(0.5f, 0.f);
	vertices[1].normal = DirectX::XMFLOAT3(0.f, 0.f, -1.f);

	vertices[2].position = DirectX::XMFLOAT3(1.f, -1.f, 0.f); //Bottom Right
	vertices[2].texture = DirectX::XMFLOAT2(1.f, 1.f);
	vertices[2].normal = DirectX::XMFLOAT3(0.f, 0.f, -1.f);

	indices[0] = 0; //Bottom Left
	indices[1] = 1; //Top Middle
	indices[2] = 2; //Bottom Right
	
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if ( FAILED(result) )
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if ( FAILED(result) )
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;
	delete[] indices;
	indices = nullptr;

	return true;

}

void Model::shutDownBuffers( )
{
	if ( nullptr != mIndexBuffer )
	{
		mIndexBuffer->Release( );
		mIndexBuffer = nullptr;
	}
	if ( nullptr != mVertexBuffer )
	{
		mVertexBuffer->Release( );
		mVertexBuffer = nullptr;
	}
}

void Model::renderBuffers( ID3D11DeviceContext* deviceContext )
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;
	deviceContext->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );
	deviceContext->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool Model::loadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fileName)
{
	mTexture = new Texture;
	if ( nullptr == mTexture )
	{
		return false;
	}

	if ( false == mTexture->initialize(device, deviceContext, fileName) )
	{
		return false;
	}

	return true;
}

void Model::releaseTexture()
{
	if ( nullptr != mTexture )
	{
		mTexture->shutDown();
		delete mTexture;
		mTexture = nullptr;
	}
}
