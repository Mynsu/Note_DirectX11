#pragma once

#include <d3d11.h>
//#include <d3dx11tex.h>
#pragma comment(lib, "DirectXTex")
#include "DirectXTex.h"

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool initialize(ID3D11Device*, WCHAR*);
	void shutDown();

	ID3D11ShaderResourceView* getTexture();

private:
	ID3D11ShaderResourceView* mTexture;
};
