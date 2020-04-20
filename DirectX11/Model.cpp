#include "Model.h"
#include <fstream>
#include "Texture.h"

bool Model::initializeBuffers( ID3D11Device* device )
{
	VertexType* vertices = new VertexType[mVertexCount];
	if ( nullptr == vertices )
	{
		return false;
	}

	unsigned long* indices = new unsigned long[mIndexCount];
	if ( nullptr == indices )
	{
		return false;
	}

	for ( int i = 0; i < mVertexCount; ++i )
	{
		vertices[i].position = DirectX::XMFLOAT3(mModel[i].x, mModel[i].y, mModel[i].z);
		vertices[i].texture = DirectX::XMFLOAT2(mModel[i].tu, mModel[i].tv);
		vertices[i].normal = DirectX::XMFLOAT3(mModel[i].nx, mModel[i].ny, mModel[i].nz);
		indices[i] = i;
	}
		
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

bool Model::loadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
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

bool Model::loadModel(char* fileName)
{
	std::ifstream fin;
	fin.open(fileName);
	if ( fin.fail() )
	{
		return false;
	}

	char input;
	fin.get(input);
	while ( input != ':' )
	{
		fin.get(input);
	}

	fin >> mVertexCount;
	
	mIndexCount = mVertexCount;

	mModel = new ModelType[mVertexCount];
	if ( nullptr == mModel )
	{
		return false;
	}

	fin.get(input);
	while ( input != ':' )
	{
		fin.get(input);
	}
	fin.get(input);//
	fin.get(input);//

	for ( int i = 0; i < mVertexCount; ++i )
	{
		fin >> mModel[i].x >> mModel[i].y >> mModel[i].z;
		fin >> mModel[i].tu >> mModel[i].tv;
		fin >> mModel[i].nx >> mModel[i].ny >> mModel[i].nz;
	}
	
	fin.close( );

	return true;
}

void Model::releaseModel()
{
	if ( nullptr != mModel )
	{
		delete[] mModel;
		mModel = nullptr;
	}
}
