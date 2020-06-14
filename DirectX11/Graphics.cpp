#include "graphics.h"

Graphics::Graphics()
{
	mD3D = 0;
	mCamera = 0;
	mText = 0;
}


Graphics::Graphics(const Graphics& other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;


	// Create the Direct3D object.
	mD3D = new D3D;
	if(!mD3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = mD3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	mCamera = new Camera;
	if(!mCamera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	mCamera->setPosition(0.0f, 0.0f, -1.0f);
	mCamera->render();
	mCamera->getViewMatrix(baseViewMatrix);

	// Create the text object.
	mText = new Text;
	if(!mText)
	{
		return false;
	}

	// Initialize the text object.
	result = mText->initialize(mD3D->getDevice(), mD3D->getDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::shutDown()
{
	// Release the text object.
	if(mText)
	{
		mText->shutDown();
		delete mText;
		mText = 0;
	}

	// Release the camera object.
	if(mCamera)
	{
		delete mCamera;
		mCamera = 0;
	}

	// Release the D3D object.
	if(mD3D)
	{
		mD3D->shutDown();
		delete mD3D;
		mD3D = 0;
	}

	return;
}


void Graphics::frame()
{
	// Set the position of the camera.
	mCamera->setPosition(0.0f, 0.0f, -10.0f);

	return;
}


bool Graphics::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	mD3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	mCamera->render();

	// Get the view, projection, and world matrices from the camera and D3D objects.
	mCamera->getViewMatrix(viewMatrix);
	mD3D->getWorldMatrix(worldMatrix);
	mD3D->getProjectionMatrix(projectionMatrix);
	mD3D->getOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	mD3D->turnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	mD3D->turnOnAlphaBlending();

	// Render the text strings.
	result = mText->render(mD3D->getDeviceContext(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	mD3D->turnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	mD3D->turnZBufferOn();

	// Present the rendered scene to the screen.
	mD3D->endScene();

	return true;
}