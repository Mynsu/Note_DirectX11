#include "text.h"
#include <string>

Text::Text()
{
	mFont = 0;
	mFontShader = 0;

	mSentence1 = 0;
	mSentence2 = 0;
	mSentence3 = 0;
}


Text::Text(const Text& other)
{
}


Text::~Text()
{
}


bool Text::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, 
						   XMMATRIX baseViewMatrix)
{
	bool result;


	// Store the screen width and height.
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Store the base view matrix.
	mBaseViewMatrix = baseViewMatrix;

	// Create the font object.
	mFont = new Font;
	if(!mFont)
	{
		return false;
	}

	// Initialize the font object.
	CHAR fontDataPath[] = "data/fontData.txt";
	WCHAR fontTexturePath[] = L"data/fontTexture.tga";
	result = mFont->initialize(device, fontDataPath, fontTexturePath);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	mFontShader = new FontShader;
	if(!mFontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = mFontShader->initialize(device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	result = initializeSentence(&mSentence1, 16, device);
	if(!result)
	{
		return false;
	}

	//// Now update the sentence vertex buffer with the new string information.
	//CHAR msg[] = "Hello";
	//result = updateSentence(mSentence1, msg, 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	//if(!result)
	//{
	//	return false;
	//}

	// Initialize the first sentence.
	result = initializeSentence(&mSentence2, 16, device);
	if(!result)
	{
		return false;
	}

	//// Now update the sentence vertex buffer with the new string information.
	//CHAR msg2[] = "Goodbye";
	//result = updateSentence(mSentence2, msg2, 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	//if(!result)
	//{
	//	return false;
	//}

	result = initializeSentence(&mSentence3, 16, device);
	if(!result)
	{
		return false;
	}

	return true;
}


void Text::shutDown()
{
	// Release the first sentence.
	releaseSentence(&mSentence1);

	// Release the second sentence.
	releaseSentence(&mSentence2);

	releaseSentence(&mSentence3);

	// Release the font shader object.
	if(mFontShader)
	{
		mFontShader->shutDown();
		delete mFontShader;
		mFontShader = 0;
	}

	// Release the font object.
	if(mFont)
	{
		mFont->shutDown();
		delete mFont;
		mFont = 0;
	}

	return;
}


bool Text::render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
	bool result;


	// Draw the first sentence.
	result = renderSentence(deviceContext, mSentence1, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Draw the second sentence.
	result = renderSentence(deviceContext, mSentence2, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	result = renderSentence(deviceContext, mSentence3, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	return true;
}

bool Text::setMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;


	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = updateSentence(mSentence1, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = updateSentence(mSentence2, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}

bool Text::setKeysPressed(unsigned char key, ID3D11DeviceContext* deviceContext)
{
	char keyboardString[16];
	bool result;

	strcpy_s(keyboardString, "Keyboard: ");
	strcat_s(keyboardString, std::to_string(key).data());

	// Update the sentence vertex buffer with the new string information.
	result = updateSentence(mSentence3, keyboardString, 20, 60, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}


bool Text::initializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool Text::updateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
							   ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((mScreenWidth / 2) * -1) + positionX);
	drawY = (float)((mScreenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	mFont->buildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void Text::releaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool Text::renderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX worldMatrix, 
							   XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = mFontShader->render(deviceContext, sentence->indexCount, worldMatrix, mBaseViewMatrix, orthoMatrix, mFont->getTexture(), 
								  pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}