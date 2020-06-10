#include "Font.h"
#include <fstream>

bool Font::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, char* textureFilename)
{
	bool result = loadFontData(fontFilename);
	if ( false == result)
	{
		return false;
	}

	result = loadTexture(device, deviceContext, textureFilename);
	if ( false == result)
	{
		return false;
	}

	return true;
}

bool Font::loadFontData(char* filename)
{
	std::ifstream fin;
	mFont = new FontType[95];
	if ( nullptr == mFont )
	{
		return false;
	}
	fin.open(filename);
	if ( fin.fail() )
	{
		return false;
	}

	for ( int i = 0; i < 95; ++i )
	{
		char temp;
		fin.get(temp);
		while ( temp != ' ' )
		{
			fin.get(temp);
		}
		fin.get(temp);
		while ( temp != ' ' )
		{
			fin.get(temp);
		}
		fin >> mFont[i].left;
		fin >> mFont[i].right;
		fin >> mFont[i].size;
	}

	return true;
}

bool Font::loadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	mTexture = new Texture;
	if( nullptr == mTexture )
	{
		return false;
	}
	bool result = mTexture->initialize(device, deviceContext, filename);
	if( false == result)
	{
		return false;
	}

	return true;
}

void Font::buildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr = (VertexType*)vertices;
	int numLetters = (int)std::strlen(sentence);
	int	index = 0;
	// Draw each letter onto a quad.
	for ( int i = 0; i < numLetters; ++i )
	{
		int letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if ( 0 == letter )
		{
			drawX += 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(mFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3((drawX + mFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(mFont[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(mFont[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(mFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX + mFont[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(mFont[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3((drawX + mFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(mFont[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX += mFont[letter].size + 1.0f;
		}
	}
}