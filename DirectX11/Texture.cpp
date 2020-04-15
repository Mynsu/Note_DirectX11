#include "Texture.h"
#include <stdio.h>

bool Texture::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fileName)
{
	int height, width;
	bool result = loadTarga(fileName, height, width);
	if ( false == result )
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	HRESULT hResult = device->CreateTexture2D(&textureDesc, NULL, &mTexture);
	if ( FAILED(hResult) )
	{
		return false;
	}

	unsigned int rowPitch = (width*4)*sizeof(unsigned char);
	// UpdateSubresource는 1회성.  캐시에 넣기 때문.  D3D11_USAGE_DEFAULT와 쓰임.
	// Map/Unmap은 반복성.  메모리에 넣기 때문.  D3D11_USAGE_DYNAMIC과 쓰임.
	deviceContext->UpdateSubresource(mTexture, 0, NULL, mTargaData, rowPitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1; // UINT_MAX
	hResult = device->CreateShaderResourceView(mTexture, &srvDesc, &mTextureView);
	if ( FAILED(hResult) )
	{
		return false;
	}
	deviceContext->GenerateMips(mTextureView);

	delete[] mTargaData;
	mTargaData = nullptr;

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

bool Texture::loadTarga(const char* fileName, int& width, int& height)
{
	FILE* filePtr;
	int error = fopen_s(&filePtr, fileName, "rb" );
	if ( 0 != error )
	{
		return false;
	}

	TargaHeader targaFileHeader;
	unsigned int count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if ( 1 != count )
	{
		return false;
	}

	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	int bpp = (int)targaFileHeader.bpp; //bit per pixel
	if ( 32 != bpp )
	{
		return false;
	}
	int imageSize = width*height*4; //4 bytes
	unsigned char* targaImage = new unsigned char[imageSize];
	if ( nullptr == targaImage )
	{
		return false;
	}

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if ( count != imageSize )
	{
		return false;
	}

	error = fclose(filePtr);
	if ( 0 != error )
	{
		return false;
	}

	mTargaData = new unsigned char[imageSize];
	if ( nullptr == mTargaData )
	{
		return false;
	}

	int index = 0;
	int k = width*height*4 - width*4;
	for ( int j = 0; j < height; ++j )
	{
		for ( int i = 0; i < width; ++i )
		{
			mTargaData[index + 0] = targaImage[k + 2]; //R
			mTargaData[index + 1] = targaImage[k + 1]; //G
			mTargaData[index + 2] = targaImage[k + 0]; //B
			mTargaData[index + 3] = targaImage[k + 3]; //A

			k += 4;
			index += 4;
		}
		k -= width*8;
	}
	delete[] targaImage;
	targaImage = nullptr;

	return true;
}
