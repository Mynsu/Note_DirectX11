#include "Bitmap.h"
#include <fstream>
#include "Texture.h"

bool Bitmap::initialize(ID3D11Device* device,
						int screenWidth, int screenHeight, WCHAR* textureFileName, int bitmapWidth, int bitmapHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mBitmapWidth = bitmapWidth;
	mBitmapHeight = bitmapHeight;
	mPreviousPosX = mPreviousPosY = -1;
	/*if ( false == loadModel(modelFileName) )
	{
		return false;
	}*/

	if ( false == initializeBuffers(device) )
	{
		return false;
	}

	if ( false == loadTexture(device, textureFileName) )
	{
		return false;
	}

	return true;
}

bool Bitmap::render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	if ( false == updateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}
	renderBuffers( deviceContext );

	return true;
}

bool Bitmap::initializeBuffers( ID3D11Device* device )
{
	mVertexCount = 6;
	mIndexCount = 6;
	//const uint32_t nVertices = (int)mModel.size();
	//VertexType* vertices = new VertexType[nVertices];
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
	ZeroMemory(vertices, sizeof(VertexType)*mVertexCount);
	for ( int i = 0; i != mIndexCount; ++i )
	{
		indices[i] = i;
	}

	/*for ( uint32_t i = 0; i < nVertices; ++i )
	{
		vertices[i].position = DirectX::XMFLOAT3(mModel[i].x, mModel[i].y, mModel[i].z);
		vertices[i].texture = DirectX::XMFLOAT2(mModel[i].tu, mModel[i].tv);
		vertices[i].normal = DirectX::XMFLOAT3(mModel[i].nx, mModel[i].ny, mModel[i].nz);
	}*/

	D3D11_BUFFER_DESC vertexBufferDesc;
	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//vertexBufferDesc.ByteWidth = sizeof(VertexType) * nVertices;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * (UINT)mIndices.size();
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	//indexData.pSysMem = mIndices.data();
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

void Bitmap::shutDownBuffers( )
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

bool Bitmap::updateBuffers(ID3D11DeviceContext * deviceContext, int positionX, int positionY)
{
	if ( (positionX == mPreviousPosX) && (positionY == mPreviousPosY) )
	{
		return true;
	}
	mPreviousPosX = positionX;
	mPreviousPosY = positionY;

	float left = (float)(mScreenWidth*-.5f) + positionX;
	float right = left + (float)mBitmapWidth;
	float top = (float)(mScreenHeight*.5f) - positionY;
	float bottom = top - (float)mBitmapHeight;

	VertexType* vertices = nullptr;
	vertices = new VertexType[mVertexCount];
	if ( nullptr == vertices )
	{
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(left, top, 0.f);
	vertices[0].texture = DirectX::XMFLOAT2(0.f, 0.f);
	vertices[1].position = DirectX::XMFLOAT3(right, bottom, 0.f);
	vertices[1].texture = DirectX::XMFLOAT2(1.f, 1.f);
	vertices[2].position = DirectX::XMFLOAT3(left, bottom, 0.f);
	vertices[2].texture = DirectX::XMFLOAT2(0.f, 1.f);

	vertices[3].position = DirectX::XMFLOAT3(left, top, 0.f);
	vertices[3].texture = DirectX::XMFLOAT2(0.f, 0.f);
	vertices[4].position = DirectX::XMFLOAT3(right, top, 0.f);
	vertices[4].texture = DirectX::XMFLOAT2(1.f, 0.f);
	vertices[5].position = DirectX::XMFLOAT3(right, bottom, 0.f);
	vertices[5].texture = DirectX::XMFLOAT2(1.f, 1.f);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // Locking
	if ( FAILED(result) )
	{
		return false;
	}
	VertexType* verticesPtr = (VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, sizeof(VertexType)*mVertexCount);
	deviceContext->Unmap(mVertexBuffer, 0); // Unlocking

	delete[] vertices;
	vertices = nullptr;

	return true;
}

void Bitmap::renderBuffers( ID3D11DeviceContext* deviceContext )
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;
	deviceContext->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );
	deviceContext->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool Bitmap::loadTexture(ID3D11Device* device, WCHAR* fileName)
{
	mTexture = new Texture;
	if ( nullptr == mTexture )
	{
		return false;
	}

	if ( false == mTexture->initialize(device, fileName) )
	{
		return false;
	}

	return true;
}

void Bitmap::releaseTexture()
{
	if ( nullptr != mTexture )
	{
		mTexture->shutDown();
		delete mTexture;
		mTexture = nullptr;
	}
}

//bool Bitmap::loadModel(char* fileName)
//{
//	Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(fileName,
//											 aiProcess_CalcTangentSpace |
//											 aiProcess_Triangulate |
//											 aiProcess_JoinIdenticalVertices |
//											 aiProcess_SortByPType |
//											 aiProcess_ConvertToLeftHanded |
//											 aiProcess_ValidateDataStructure |
//											 aiProcess_ImproveCacheLocality |
//											 aiProcess_RemoveRedundantMaterials |
//											 aiProcess_GenUVCoords |
//											 aiProcess_TransformUVCoords |
//											 aiProcess_FindInstances |
//											 aiProcess_LimitBoneWeights |
//											 aiProcess_OptimizeMeshes |
//											 aiProcess_GenSmoothNormals |
//											 aiProcess_SplitLargeMeshes);
//	if ( nullptr == scene )
//	{
//		return false;
//	}
//
//	const uint32_t nMeshes = scene->mNumMeshes;
//	for ( uint32_t i = 0; nMeshes != i; ++i )
//	{
//		const aiMesh* mesh = scene->mMeshes[i];
//		const uint32_t nVerticesAMesh = mesh->mNumVertices;
//		mModel.reserve(nVerticesAMesh);
//		for ( uint32_t i = 0; i < nVerticesAMesh; ++i )
//		{
//			ModelType modelType;
//			modelType.x = mesh->mVertices[i].x;
//			modelType.y = mesh->mVertices[i].y;
//			modelType.z = mesh->mVertices[i].z;
//			modelType.nx = mesh->mNormals[i].x;
//			modelType.ny = mesh->mNormals[i].y;
//			modelType.nz = mesh->mNormals[i].z;
//			if ( mesh->HasTextureCoords(0) )
//			{
//				modelType.tu = mesh->mTextureCoords[0][i].x;
//				modelType.tv = mesh->mTextureCoords[0][i].y;
//			}
//			else
//			{
//				modelType.tu = modelType.tv = 0.f;
//			}
//			mModel.emplace_back(modelType);
//		}
//		const uint32_t nFacesAMesh = mesh->mNumFaces;
//		mIndices.reserve(nFacesAMesh*3);
//		for ( uint32_t i = 0; nFacesAMesh != i; ++i )
//		{
//			const aiFace& face = mesh->mFaces[i];
//			mIndices.emplace_back(face.mIndices[0]);
//			mIndices.emplace_back(face.mIndices[1]);
//			mIndices.emplace_back(face.mIndices[2]);
//		}
//	}
//
//	return true;
//}