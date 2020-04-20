#include "Texture.h"
#include <stdio.h>

bool Texture::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
{
	DirectX::ScratchImage image = loadTextureFromFile(fileName);
	HRESULT result = DirectX::CreateShaderResourceView(device,
													   image.GetImages(), image.GetImageCount(),
													   image.GetMetadata(), &mTextureView);
	if ( FAILED(result) )
	{
		return false;
	}

	return true;
}

void Texture::shutDown()
{
	if ( nullptr != mTextureView )
	{
		mTextureView->Release();
		mTextureView = nullptr;
	}
	if ( nullptr != mTexture )
	{
		mTexture->Release();
		mTexture = nullptr;
	}
	if ( nullptr != mTargaData )
	{
		delete[] mTargaData;
		mTargaData = nullptr;
	}
}

DirectX::ScratchImage Texture::loadTextureFromFile(LPCSTR fileName)
{
	std::string str(fileName);
	std::wstring wsTmp(str.begin(), str.end());
	std::wstring ws( wsTmp );
	WCHAR ext[_MAX_EXT];
	_wsplitpath_s(ws.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
	HRESULT hr;
	DirectX::ScratchImage image;
	if ( 0 == _wcsicmp(ext, L".dds") )
	{
		hr = DirectX::LoadFromDDSFile(ws.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if ( 0 == _wcsicmp(ext, L".tga") )
	{
		hr = DirectX::LoadFromTGAFile(ws.c_str(), nullptr, image);
	}
	else if ( 0 == _wcsicmp(ext, L".wic") )
	{
		hr = DirectX::LoadFromWICFile(ws.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
	}

	return image;
}