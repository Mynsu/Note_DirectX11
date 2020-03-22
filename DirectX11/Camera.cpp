#include "Camera.h"

void Camera::render( )
{
	DirectX::XMFLOAT3 up;
	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;
	DirectX::XMVECTOR upVector = DirectX::XMLoadFloat3(&up);

	DirectX::XMFLOAT3 position;
	position.x = mPositionX;
	position.y = mPositionY;
	position.z = mPositionZ;
	DirectX::XMVECTOR positionVector = XMLoadFloat3(&position);

	DirectX::XMFLOAT3 lookAt;
	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;
	DirectX::XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	float pitch = mRotationX * 0.0174532925f;
	float yaw = mRotationY * 0.0174532925f;
	float roll = mRotationZ * 0.0174532925f;
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = DirectX::XMVector3TransformCoord(upVector, rotationMatrix);
	
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	mViewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}
