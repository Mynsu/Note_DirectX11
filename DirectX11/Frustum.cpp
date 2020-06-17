#include "Frustum.h"

Frustum::Frustum()
{
}


Frustum::Frustum(const Frustum& other)
{
}


Frustum::~Frustum()
{
}

void Frustum::constructFrustum(float screenDepth, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix)
{
	float zMinimum, r;
	DirectX::XMMATRIX matrix;

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -DirectX::XMVectorGetZ(projectionMatrix.r[3]) / DirectX::XMVectorGetZ(projectionMatrix.r[2]);
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix.r[2] = DirectX::XMVectorSetZ(projectionMatrix.r[2], r);
	projectionMatrix.r[3] = DirectX::XMVectorSetZ(projectionMatrix.r[3], -r*zMinimum);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	matrix = DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix);

	// Calculate near plane of frustum.
	mPlanes[0] = DirectX::XMVectorSetX(mPlanes[0], DirectX::XMVectorGetW(matrix.r[0]) + DirectX::XMVectorGetZ(matrix.r[0]));
	mPlanes[0] = DirectX::XMVectorSetY(mPlanes[0], DirectX::XMVectorGetW(matrix.r[1]) + DirectX::XMVectorGetZ(matrix.r[1]));
	mPlanes[0] = DirectX::XMVectorSetZ(mPlanes[0], DirectX::XMVectorGetW(matrix.r[2]) + DirectX::XMVectorGetZ(matrix.r[2]));
	mPlanes[0] = DirectX::XMVectorSetW(mPlanes[0], DirectX::XMVectorGetW(matrix.r[3]) + DirectX::XMVectorGetZ(matrix.r[3]));
	mPlanes[0] = DirectX::XMPlaneNormalize(mPlanes[0]);

	// Calculate far plane of frustum.
	mPlanes[1] = DirectX::XMVectorSetX(mPlanes[1], DirectX::XMVectorGetW(matrix.r[0]) - DirectX::XMVectorGetZ(matrix.r[0]));
	mPlanes[1] = DirectX::XMVectorSetY(mPlanes[1], DirectX::XMVectorGetW(matrix.r[1]) - DirectX::XMVectorGetZ(matrix.r[1]));
	mPlanes[1] = DirectX::XMVectorSetZ(mPlanes[1], DirectX::XMVectorGetW(matrix.r[2]) - DirectX::XMVectorGetZ(matrix.r[2]));
	mPlanes[1] = DirectX::XMVectorSetW(mPlanes[1], DirectX::XMVectorGetW(matrix.r[3]) - DirectX::XMVectorGetZ(matrix.r[3]));
	mPlanes[1] = DirectX::XMPlaneNormalize(mPlanes[1]);

	// Calculate left plane of frustum.
	mPlanes[2] = DirectX::XMVectorSetX(mPlanes[2], DirectX::XMVectorGetW(matrix.r[0]) + DirectX::XMVectorGetX(matrix.r[0]));
	mPlanes[2] = DirectX::XMVectorSetY(mPlanes[2], DirectX::XMVectorGetW(matrix.r[1]) + DirectX::XMVectorGetX(matrix.r[1]));
	mPlanes[2] = DirectX::XMVectorSetZ(mPlanes[2], DirectX::XMVectorGetW(matrix.r[2]) + DirectX::XMVectorGetX(matrix.r[2]));
	mPlanes[2] = DirectX::XMVectorSetW(mPlanes[2], DirectX::XMVectorGetW(matrix.r[3]) + DirectX::XMVectorGetX(matrix.r[3]));
	mPlanes[2] = DirectX::XMPlaneNormalize(mPlanes[2]);

	// Calculate right plane of frustum.
	mPlanes[3] = DirectX::XMVectorSetX(mPlanes[3], DirectX::XMVectorGetW(matrix.r[0]) - DirectX::XMVectorGetX(matrix.r[0]));
	mPlanes[3] = DirectX::XMVectorSetY(mPlanes[3], DirectX::XMVectorGetW(matrix.r[1]) - DirectX::XMVectorGetX(matrix.r[1]));
	mPlanes[3] = DirectX::XMVectorSetZ(mPlanes[3], DirectX::XMVectorGetW(matrix.r[2]) - DirectX::XMVectorGetX(matrix.r[2]));
	mPlanes[3] = DirectX::XMVectorSetW(mPlanes[3], DirectX::XMVectorGetW(matrix.r[3]) - DirectX::XMVectorGetX(matrix.r[3]));
	mPlanes[3] = DirectX::XMPlaneNormalize(mPlanes[3]);

	// Calculate top plane of frustum.
	mPlanes[4] = DirectX::XMVectorSetX(mPlanes[4], DirectX::XMVectorGetW(matrix.r[0]) - DirectX::XMVectorGetY(matrix.r[0]));
	mPlanes[4] = DirectX::XMVectorSetY(mPlanes[4], DirectX::XMVectorGetW(matrix.r[1]) - DirectX::XMVectorGetY(matrix.r[1]));
	mPlanes[4] = DirectX::XMVectorSetZ(mPlanes[4], DirectX::XMVectorGetW(matrix.r[2]) - DirectX::XMVectorGetY(matrix.r[2]));
	mPlanes[4] = DirectX::XMVectorSetW(mPlanes[4], DirectX::XMVectorGetW(matrix.r[3]) - DirectX::XMVectorGetY(matrix.r[3]));
	mPlanes[4] = DirectX::XMPlaneNormalize(mPlanes[4]);

	// Calculate bottom plane of frustum.
	mPlanes[5] = DirectX::XMVectorSetX(mPlanes[5], DirectX::XMVectorGetW(matrix.r[0]) + DirectX::XMVectorGetY(matrix.r[0]));
	mPlanes[5] = DirectX::XMVectorSetY(mPlanes[5], DirectX::XMVectorGetW(matrix.r[1]) + DirectX::XMVectorGetY(matrix.r[1]));
	mPlanes[5] = DirectX::XMVectorSetZ(mPlanes[5], DirectX::XMVectorGetW(matrix.r[2]) + DirectX::XMVectorGetY(matrix.r[2]));
	mPlanes[5] = DirectX::XMVectorSetW(mPlanes[5], DirectX::XMVectorGetW(matrix.r[3]) + DirectX::XMVectorGetY(matrix.r[3]));
	mPlanes[5] = DirectX::XMPlaneNormalize(mPlanes[5]);

	return;
}

bool Frustum::checkPoint(float x, float y, float z)
{
	int i;


	// Check if the point is inside all six planes of the view frustum.
	for(i=0; i<6; i++) 
	{
		DirectX::XMFLOAT3 temp(x, y, z);
		DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
		float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
		if ( dot < 0.f )
		{
			return false;
		}
	}

	return true;
}

bool Frustum::checkCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if any one point of the cube is in the view frustum.
	for(i=0; i<6; i++) 
	{
		{
			DirectX::XMFLOAT3 temp(xCenter - radius, yCenter - radius, zCenter - radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + radius, yCenter - radius, zCenter - radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter - radius, yCenter + radius, zCenter - radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + radius, yCenter + radius, zCenter - radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter - radius, yCenter - radius, zCenter + radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + radius, yCenter - radius, zCenter + radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter - radius, yCenter + radius, zCenter + radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + radius, yCenter + radius, zCenter + radius);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		return false;
	}

	return true;
}

bool Frustum::checkSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if the radius of the sphere is inside the view frustum.
	for(i=0; i<6; i++) 
	{
		DirectX::XMFLOAT3 temp(xCenter, yCenter, zCenter);
		DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
		float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
		if(dot < -radius)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::checkRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	int i;


	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for(i=0; i<6; i++)
	{
		{
			DirectX::XMFLOAT3 temp(xCenter - xSize, yCenter - ySize, zCenter - zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + xSize, yCenter - ySize, zCenter - zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter - xSize, yCenter + ySize, zCenter - zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter - xSize, yCenter - ySize, zCenter + zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + xSize, yCenter + ySize, zCenter - zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + xSize, yCenter - ySize, zCenter + zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter - xSize, yCenter + ySize, zCenter + zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		{
			DirectX::XMFLOAT3 temp(xCenter + xSize, yCenter + ySize, zCenter + zSize);
			DirectX::XMVECTOR dotV = DirectX::XMPlaneDotCoord(mPlanes[i], DirectX::XMLoadFloat3(&temp));
			float dot = DirectX::XMVectorGetX(dotV) + DirectX::XMVectorGetY(dotV) + DirectX::XMVectorGetZ(dotV) + DirectX::XMVectorGetW(dotV);
			if(dot >= 0.0f)
			{
				continue;
			}
		}

		return false;
	}

	return true;
}