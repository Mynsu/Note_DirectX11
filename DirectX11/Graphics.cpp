#include "graphics.h"

Graphics::Graphics()
{
	mD3D = 0;
	mCamera = 0;
	mText = 0;
	mModel = 0;
	mLightShader = 0;
	mLight = 0;
	mModelList = 0;
	mFrustum = 0;
}


Graphics::Graphics(const Graphics& other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::initialize(int screenWidth, int screenHeight, HWND hWnd)
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
	result = mD3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
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
	result = mText->initialize(mD3D->getDevice(), mD3D->getDeviceContext(), hWnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	mModel = new Model;
	if(!mModel)
	{
		return false;
	}

	// Initialize the model object.
	WCHAR texturePath[] = L"data/Diffuse_2K.tga";
	char modelPath[] = "data/Moon_2K.fbx";
	result = mModel->initialize(mD3D->getDevice(), texturePath, modelPath);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	mLightShader = new LightShader;
	if(!mLightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = mLightShader->initialize(mD3D->getDevice(), hWnd);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	mLight = new Light;
	if(!mLight)
	{
		return false;
	}

	// Initialize the light object.
	mLight->setDirection(0.0f, 0.0f, 1.0f);

	// Create the model list object.
	mModelList = new ModelList;
	if(!mModelList)
	{
		return false;
	}

	// Initialize the model list object.
	result = mModelList->initialize(25);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	mFrustum = new Frustum;
	if(!mFrustum)
	{
		return false;
	}

	return true;
}


void Graphics::shutDown()
{
	// Release the frustum object.
	if(mFrustum)
	{
		delete mFrustum;
		mFrustum = 0;
	}

	// Release the model list object.
	if(mModelList)
	{
		mModelList->shutDown();
		delete mModelList;
		mModelList = 0;
	}

	// Release the light object.
	if(mLight)
	{
		delete mLight;
		mLight = 0;
	}

	// Release the light shader object.
	if(mLightShader)
	{
		mLightShader->shutDown();
		delete mLightShader;
		mLightShader = 0;
	}

	// Release the model object.
	if(mModel)
	{
		mModel->shutDown();
		delete mModel;
		mModel = 0;
	}

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


///bool Graphics::frame(int fps, int cpu, float frameTime)
bool Graphics::frame(float rotationY)
{
	///bool result;

	/// Set the location of the mouse.
	///result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	///if(!result)
	///{
	///	return false;
	///}

	/// Set the frames per second.
	///result = mText->setFPS(fps, mD3D->getDeviceContext());
	///if(!result)
	///{
	///	return false;
	///}

	/// Set the cpu usage.
	///result = mText->setCPU(cpu, mD3D->getDeviceContext());
	///if(!result)
	///{
	///	return false;
	///}

	// Set the position of the camera.
	mCamera->setPosition(0.0f, 0.0f, -10.0f);

	// Set the rotation of the camera.
	mCamera->setRotation(0.0f, rotationY, 0.0f);

	return true;
}


bool Graphics::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	XMFLOAT4 color;
	bool renderModel, result;


	// Clear the buffers to begin the scene.
	mD3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	mCamera->render();

	// Get the view, projection, and world matrices from the camera and D3D objects.
	mCamera->getViewMatrix(viewMatrix);
	mD3D->getWorldMatrix(worldMatrix);
	mD3D->getProjectionMatrix(projectionMatrix);
	mD3D->getOrthoMatrix(orthoMatrix);

	// Construct the frustum.
	mFrustum->constructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of models that will be rendered.
	modelCount = mModelList->getModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index<modelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		mModelList->getData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;

		// Check if the sphere model is in the view frustum.
		renderModel = mFrustum->checkSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if(renderModel)
		{
			// Move the model to the location it should be rendered at.
			worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ); 

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			mModel->render(mD3D->getDeviceContext());

			// Render the model using the light shader.
			mLightShader->render(mD3D->getDeviceContext(), mModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								mModel->getTexture(), mLight->getDirection(), color/*,
								 {0.7f, 0.7f, 0.7f, 0.7f}, mCamera->getPosition(), {1.f, 1.f, 1.f, 1.f}, 1.f*/ );

			// Reset to the original world matrix.
			mD3D->getWorldMatrix(worldMatrix);

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}
	}

	// Set the number of models that was actually rendered this frame.
	result = mText->setRenderCount(renderCount, mD3D->getDeviceContext());
	if(!result)
	{
		return false;
	}

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