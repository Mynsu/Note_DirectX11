#include "Camera.h"

Camera::Camera()
{
	mPositionX = 0.0f;
	mPositionY = 0.0f;
	mPositionZ = 0.0f;

	mRotationX = 0.0f;
	mRotationY = 0.0f;
	mRotationZ = 0.0f;
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::setPosition(float x, float y, float z)
{
	mPositionX = x;
	mPositionY = y;
	mPositionZ = z;
	return;
}


void Camera::setRotation(float x, float y, float z)
{
	mRotationX = x;
	mRotationY = y;
	mRotationZ = z;
	return;
}


void Camera::render()
{
	XMFLOAT3 up;
	XMFLOAT3 position;
	XMFLOAT3 lookAt;
	float radians;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = mPositionX;
	position.y = mPositionY;
	position.z = mPositionZ;

	// Calculate the rotation in radians.
	radians = mRotationY * 0.0174532925f;

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + mPositionX;
	lookAt.y = mPositionY;
	lookAt.z = cosf(radians) + mPositionZ;

	// Create the view matrix from the three vectors.
	XMVECTOR vPos = XMLoadFloat3(&position);
	XMVECTOR vLookAt = XMLoadFloat3(&lookAt);
	XMVECTOR vUp = XMLoadFloat3(&up);
	mViewMatrix = XMMatrixLookAtLH(vPos, vLookAt, vUp);
	return;
}


void Camera::getViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = mViewMatrix;
	return;
}