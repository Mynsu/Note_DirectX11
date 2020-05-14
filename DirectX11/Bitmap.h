#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"
#pragma comment(lib, "assimp-vc141-mtd")

class Bitmap
{
public:
	Bitmap( )
		: mVertexBuffer( nullptr ), mIndexBuffer( nullptr ), mTexture( nullptr )
	{}
	Bitmap( const Bitmap& ) = delete;
	~Bitmap( ) = default;
	bool initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext,
					int screenWidth, int screenHeight, char* textureFileName, int bitmapWidth, int bitmapHeight );
	void shutDown( )
	{
		shutDownBuffers( );
		releaseTexture( );
		//releaseModel( );
	}
	bool render( ID3D11DeviceContext* deviceContext, int positionX, int positionY );
	int getIndexCount( )
	{
		//return (int)mIndices.size();
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
		//DirectX::XMFLOAT3 normal;
	};
	//struct InstanceType
	//{
	//	DirectX::XMFLOAT3 position;
	//};
	/*struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};*/
	bool initializeBuffers( ID3D11Device* device );
	void shutDownBuffers( );
	bool updateBuffers( ID3D11DeviceContext* deviceContext, int positionX, int positionY );
	void renderBuffers( ID3D11DeviceContext* deviceContext );
	bool loadTexture( ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName );
	void releaseTexture( );
	//bool loadModel( char* fileName );
	//void releaseModel( );
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount, mIndexCount;
	Texture* mTexture;
	int mScreenWidth, mScreenHeight;
	int mBitmapWidth, mBitmapHeight;
	int mPreviousPosX, mPreviousPosY;
	//std::vector<ModelType> mModel;
	//std::vector<unsigned long> mIndices;
};