#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

class Font
{
public:
	Font()
		: mFont(nullptr), mTexture(nullptr)
	{}
	Font(const Font&) = delete;
	~Font() = default;

	bool initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, char* textureFilename);
	void shutdown()
	{
		releaseTexture();
		releaseFontData();
	}
	ID3D11ShaderResourceView* getTexture()
	{
		return mTexture->getTexture();
	}
	void buildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
	bool loadFontData(char* filename);
	void releaseFontData()
	{
		if ( nullptr != mFont )
		{
			delete[] mFont;
			mFont = nullptr;
		}
	}
	bool loadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void releaseTexture()
	{
		if ( nullptr != mTexture )
		{
			mTexture->shutDown();
			delete mTexture;
			mTexture = nullptr;
		}
	}

	FontType* mFont;
	Texture* mTexture;
	const UINT mCharNum = 95;
};