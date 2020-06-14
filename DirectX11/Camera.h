#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Camera
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

	Camera();
	Camera(const Camera&);
	~Camera();

	void setPosition(float, float, float);
	void setRotation(float, float, float);
	void render();
	void getViewMatrix(XMMATRIX&);
private:
	XMMATRIX mViewMatrix;
	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;
};
