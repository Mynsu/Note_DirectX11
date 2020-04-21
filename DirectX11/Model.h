#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

class Model
{
public:
	Model( )
		: mVertexBuffer( nullptr ), mIndexBuffer( nullptr ), mTexture( nullptr ), mModel( nullptr )
	{}
	Model( const Model& ) = delete;
	~Model( ) = default;
	bool initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName, char* modelFileName )
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
	void shutDown( )
	{
		shutDownBuffers( );
		releaseTexture( );
		releaseModel( );
	}
	void render( ID3D11DeviceContext* deviceContext )
	{
		renderBuffers( deviceContext );
	}
	int getIndexCount( )
	{
		return mIndexCount;
	}
	ID3D11ShaderResourceView* getTexture( )
	{
		return mTexture->getTexture();
	}
private:
	// texture.vs의 구조체랑 같아야 함.
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};
	struct InstanceType
	{
		DirectX::XMFLOAT3 position;
	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	bool initializeBuffers( ID3D11Device* device );
	void shutDownBuffers( );
	void renderBuffers( ID3D11DeviceContext* deviceContext );
	bool loadTexture( ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName );
	void releaseTexture( );
	bool loadModel( char* fileName );
	void releaseModel( );
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount, mIndexCount;
	Texture* mTexture;
	ModelType* mModel;
};