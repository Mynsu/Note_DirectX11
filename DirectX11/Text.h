#pragma once

#include "font.h"
#include "fontshader.h"
using namespace DirectX;

class Text
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}

	Text();
	Text(const Text&);
	~Text();

	bool initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, XMMATRIX);
	void shutDown();

	bool render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX);

	bool setMousePosition(int, int, ID3D11DeviceContext*);
	bool setKeysPressed(unsigned char, ID3D11DeviceContext*);

private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	bool initializeSentence(SentenceType**, int, ID3D11Device*);
	bool updateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void releaseSentence(SentenceType**);
	bool renderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX, XMMATRIX);

	Font* mFont;
	FontShader* mFontShader;
	int mScreenWidth, mScreenHeight;
	XMMATRIX mBaseViewMatrix;
	SentenceType* mSentence1;
	SentenceType* mSentence2;
	SentenceType* mSentence3;
};
