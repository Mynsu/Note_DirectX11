#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class Model
{
public:
	Model( )
		: mVertexBuffer( nullptr ), mIndexBuffer( nullptr )
	{}
	Model( const Model& ) = delete;
	~Model( ) = default;
	bool initialize( ID3D11Device* device )
	{
		if ( false == initializeBuffers(device) )
		{
			return false;
		}

		return true;
	}
	void shutDown( )
	{
		shutDownBuffers( );
	}
	void render( ID3D11DeviceContext* deviceContext )
	{
		renderBuffers( deviceContext );
	}
	int getIndexCount( )
	{
		return mIndexCount;
	}
private:
	// texture.vs의 구조체랑 같아야 함.
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
	bool initializeBuffers( ID3D11Device* device );
	void shutDownBuffers( );
	void renderBuffers( ID3D11DeviceContext* deviceContext );
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount, mIndexCount;
};