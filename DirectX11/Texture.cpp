#include "texture.h"
///#include <assimp/Importer.hpp>
///#include <assimp/scene.h>
///#include <assimp/texture.h>

Texture::Texture()
{
	mTexture = 0;
}


Texture::Texture(const Texture& other)
{
}


Texture::~Texture()
{
}


bool Texture::initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;


	// Load the texture in.
	///Assimp::Importer importer;
	///char filePath[100];
	///sprintf(filePath, "%ws", filename);
	///const aiScene* scene = importer.ReadFile(filePath, 0);
	///scene->GetEmbeddedTexture().;

	DirectX::ScratchImage image;
	result = DirectX::LoadFromTGAFile(filename, nullptr, image);
	if(FAILED(result))
	{
		return false;
	}
	result = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &mTexture);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void Texture::shutDown()
{
	// Release the texture resource.
	if(mTexture)
	{
		mTexture->Release();
		mTexture = 0;
	}

	return;
}


ID3D11ShaderResourceView* Texture::getTexture()
{
	return mTexture;
}