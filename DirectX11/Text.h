#pragma once
#include "Font.h"
#include "FontShader.h"

class Text
{
public:
	Text()
		: mFont(nullptr), mFontShader(nullptr), mSentence1(nullptr), mSentence2(nullptr)
	{}
	Text(const Text&) = delete;
	~Text() = default;

	bool initialize(ID3D11Device* device,
					ID3D11DeviceContext* deviceContext,
					HWND hwnd,
					int screenWidth, int screenHeight, 
					DirectX::XMMATRIX baseViewMatrix);
	void shutDown()
	{
		releaseSentence(&mSentence1);
		releaseSentence(&mSentence2);

		if(mFontShader)
		{
			mFontShader->shutDown();
			delete mFontShader;
			mFontShader = nullptr;
		}

		if(mFont)
		{
			mFont->shutdown();
			delete mFont;
			mFont = nullptr;
		}
	}
	bool render(ID3D11DeviceContext* deviceContext,
				DirectX::XMMATRIX worldMatrix,
				DirectX::XMMATRIX orthoMatrix);

private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	bool initializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);
	bool updateSentence(SentenceType* sentence,
						char* text,
						int positionX, int positionY,
						float red, float green, float blue,
						ID3D11DeviceContext* deviceContext);
	void releaseSentence(SentenceType** sentence);
	bool renderSentence(ID3D11DeviceContext* deviceContext,
						SentenceType* sentence,
						DirectX::XMMATRIX worldMatrix, 
						DirectX::XMMATRIX orthoMatrix);

private:
	Font* mFont;
	FontShader* mFontShader;
	int mScreenWidth, mScreenHeight;
	DirectX::XMMATRIX mBaseViewMatrix;
	SentenceType* mSentence1;
	SentenceType* mSentence2;
};

