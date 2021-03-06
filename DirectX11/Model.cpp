#include "Model.h"

bool Model::initializeBuffers( ID3D11Device* device )
{
	mVertexCount = 8;
	mIndexCount = 9;
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

	vertices[0].position = DirectX::XMFLOAT3(-1.f, -0.f, 0.f);
	vertices[0].color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);

	vertices[1].position = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	vertices[1].color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);

	vertices[2].position = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
	vertices[2].color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);

	vertices[3].position = DirectX::XMFLOAT3(-1.f, -1.f, 0.f);
	vertices[3].color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);

	vertices[4].position = DirectX::XMFLOAT3(1.f, -1.f, 0.f);
	vertices[4].color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);

	vertices[5].position = DirectX::XMFLOAT3(0.f, -2.f, 0.f);
	vertices[5].color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

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
	HRESULT result = device->CreateBuffer( &vertexBufferDesc, &vertexData, &mVertexBuffer );
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
