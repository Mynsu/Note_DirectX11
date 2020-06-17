#pragma once

#include <DirectXMath.h>

class Frustum
{
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();

	void constructFrustum(float, DirectX::XMMATRIX, DirectX::XMMATRIX);

	bool checkPoint(float, float, float);
	bool checkCube(float, float, float, float);
	bool checkSphere(float, float, float, float);
	bool checkRectangle(float, float, float, float, float, float);

private:
	DirectX::XMVECTOR mPlanes[6];
};
