#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void setAmbientColor(float, float, float, float);
	void setDiffuseColor(float, float, float, float);
	void setSpecularColor(float, float, float, float);
	void setSpecularPower(float);
	void setDirection(float, float, float);

	XMFLOAT4 getAmbientColor();
	XMFLOAT4 getDiffuseColor();
	XMFLOAT4 getSpecularColor();
	float getSpecularPower();
	XMFLOAT3 getDirection();

private:
	XMFLOAT4 mAmbientColor;
	XMFLOAT4 mDiffuseColor;
	XMFLOAT4 mSpecularColor;
	float mSpecularPower;
	XMFLOAT3 mDirection;
};
