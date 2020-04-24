#include "Model.h"
#include <fstream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "Texture.h"

bool Model::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName, char* modelFileName)
{
	if ( false == loadModel(modelFileName) )
	{
		return false;
	}

	if ( false == initializeBuffers(device) )
	{
		return false;
	}

	if ( false == loadTexture(device, deviceContext, textureFileName) )
	{
		return false;
	}

	return true;
}

void Model::render(ID3D11DeviceContext* deviceContext)
{
	renderBuffers( deviceContext );
}

bool Model::initializeBuffers( ID3D11Device* device )
{
	const uint32_t nVertices = (int)mModel.size();
	VertexType* vertices = new VertexType[nVertices];
	if ( nullptr == vertices )
	{
		return false;
	}

	for ( uint32_t i = 0; i < nVertices; ++i )
	{
		vertices[i].position = DirectX::XMFLOAT3(mModel[i].x, mModel[i].y, mModel[i].z);
		vertices[i].texture = DirectX::XMFLOAT2(mModel[i].tu, mModel[i].tv);
		vertices[i].normal = DirectX::XMFLOAT3(mModel[i].nx, mModel[i].ny, mModel[i].nz);
	}
		
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * nVertices;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (UINT)mIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = mIndices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if ( FAILED(result) )
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

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
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
											 aiProcess_CalcTangentSpace |
											 aiProcess_Triangulate |
											 aiProcess_JoinIdenticalVertices |
											 aiProcess_SortByPType |
											 aiProcess_ConvertToLeftHanded |
											 aiProcess_ValidateDataStructure |
											 aiProcess_ImproveCacheLocality |
											 aiProcess_RemoveRedundantMaterials |
											 aiProcess_GenUVCoords |
											 aiProcess_TransformUVCoords |
											 aiProcess_FindInstances |
											 aiProcess_LimitBoneWeights |
											 aiProcess_OptimizeMeshes |
											 aiProcess_GenSmoothNormals |
											 aiProcess_SplitLargeMeshes);
	if ( nullptr == scene )
	{
		return false;
	}

	const uint32_t nMeshes = scene->mNumMeshes;
	for ( uint32_t i = 0; nMeshes != i; ++i )
	{
		const aiMesh* mesh = scene->mMeshes[i];
		const uint32_t nVerticesAMesh = mesh->mNumVertices;
		mModel.reserve(nVerticesAMesh);
		for ( uint32_t i = 0; i < nVerticesAMesh; ++i )
		{
			ModelType modelType;
			modelType.x = mesh->mVertices[i].x;
			modelType.y = mesh->mVertices[i].y;
			modelType.z = mesh->mVertices[i].z;
			modelType.nx = mesh->mNormals[i].x;
			modelType.ny = mesh->mNormals[i].y;
			modelType.nz = mesh->mNormals[i].z;
			if ( mesh->HasTextureCoords(0) )
			{
				modelType.tu = mesh->mTextureCoords[0][i].x;
				modelType.tv = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				modelType.tu = modelType.tv = 0.f;
			}
			mModel.emplace_back(modelType);
		}
		const uint32_t nFacesAMesh = mesh->mNumFaces;
		mIndices.reserve(nFacesAMesh*3);
		for ( uint32_t i = 0; nFacesAMesh != i; ++i )
		{
			const aiFace& face = mesh->mFaces[i];
			mIndices.emplace_back(face.mIndices[0]);
			mIndices.emplace_back(face.mIndices[1]);
			mIndices.emplace_back(face.mIndices[2]);
		}
	}

	return true;
}