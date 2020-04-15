#pragma once

#include <d3d11.h>

class Texture
{
public:
	Texture()
		: mTargaData( nullptr ), mTexture( nullptr ), mTextureView( nullptr )
	{}
	Texture( const Texture& ) = delete;
	~Texture() = default;
	bool initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fileName );
	void shutDown();
	ID3D11ShaderResourceView* getTexture( )
	{
		return mTextureView;
	}
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
	bool loadTarga( const char* fileName, int& width, int& height );
	unsigned char* mTargaData;
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mTextureView;
};