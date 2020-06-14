#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include "Texture.h"
using namespace DirectX;

class Font
{
public:
	Font();
	Font(const Font&);
	~Font();

	bool initialize(ID3D11Device*, char*, WCHAR*);
	void shutDown();

	ID3D11ShaderResourceView* getTexture();

	void buildVertexArray(void*, char*, float, float);
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	bool loadFontData(char*);
	void releaseFontData();
	bool loadTexture(ID3D11Device*, WCHAR*);
	void releaseTexture();

	FontType* mFont;
	Texture* mTexture;
};
