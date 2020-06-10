#include "Text.h"

bool Text::initialize(ID3D11Device* device,
					  ID3D11DeviceContext* deviceContext,
					  HWND hwnd,
					  int screenWidth, int screenHeight, 
					  DirectX::XMMATRIX baseViewMatrix)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mBaseViewMatrix = baseViewMatrix;
	mFont = new Font;
	if(!mFont)
	{
		return false;
	}
	char font[] = "data/fontData.txt";
	char fontTexture[] = "data/font.tga";
	bool result = mFont->initialize(device, deviceContext, font, fontTexture);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}
	mFontShader = new FontShader;
	if(!mFontShader)
	{
		return false;
	}
	result = mFontShader->initialize(device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	result = initializeSentence(&mSentence1, 16, device);
	if(!result)
	{
		return false;
	}
	char msg[] = "Hello";
	result = updateSentence(mSentence1, msg, 100, 100, 1.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	result = initializeSentence(&mSentence2, 16, device);
	if(!result)
	{
		return false;
	}
	char msg2[] = "Goodbye";
	result = updateSentence(mSentence2, msg2, 100, 200, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}

bool Text::render(ID3D11DeviceContext* deviceContext,
				  DirectX::XMMATRIX worldMatrix,
				  DirectX::XMMATRIX orthoMatrix)
{
	bool result = renderSentence(deviceContext, mSentence1, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}
	result = renderSentence(deviceContext, mSentence2, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	return true;
}

bool Text::initializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}
	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;
	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;

	VertexType* vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType)*(*sentence)->vertexCount));

	for(int i = 0; i < (*sentence)->indexCount; ++i)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

bool Text::updateSentence(SentenceType* sentence,
						  char* text,
						  int positionX, int positionY,
						  float red, float green, float blue,
						  ID3D11DeviceContext* deviceContext)
{
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;
	int numLetters = (int)strlen(text);
	if(numLetters > sentence->maxLength)
	{
		return false;
	}
	VertexType* vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	float drawX = (float)(((mScreenWidth / 2) * -1) + positionX);
	float drawY = (float)((mScreenHeight / 2) - positionY);
	mFont->buildVertexArray((void*)vertices, text, drawX, drawY);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}
	VertexType* verticesPtr = (VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));
	deviceContext->Unmap(sentence->vertexBuffer, 0);
	
	delete[] vertices;
	vertices = nullptr;

	return true;
}

void Text::releaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = nullptr;
		}

		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = nullptr;
		}

		delete *sentence;
		*sentence = nullptr;
	}
}

bool Text::renderSentence(ID3D11DeviceContext* deviceContext,
						  SentenceType* sentence,
						  DirectX::XMMATRIX worldMatrix, 
						  DirectX::XMMATRIX orthoMatrix)
{
	unsigned int stride = sizeof(VertexType); 
	unsigned int offset = 0;
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX::XMFLOAT4 pixelColor = DirectX::XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	bool result = mFontShader->render(deviceContext,
									  sentence->indexCount,
									  worldMatrix, mBaseViewMatrix,
									  orthoMatrix,
									  mFont->getTexture(),
									  pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}