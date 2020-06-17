#include "light.h"


Light::Light()
{
}


Light::Light(const Light& other)
{
}


Light::~Light()
{
}


void Light::setAmbientColor(float red, float green, float blue, float alpha)
{
	mAmbientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


void Light::setDiffuseColor(float red, float green, float blue, float alpha)
{
	mDiffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


void Light::setSpecularColor(float red, float green, float blue, float alpha)
{
	mSpecularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


void Light::setSpecularPower(float power)
{
	mSpecularPower = power;
	return;
}


void Light::setDirection(float x, float y, float z)
{
	mDirection = XMFLOAT3(x, y, z);
	return;
}


XMFLOAT4 Light::getAmbientColor()
{
	return mAmbientColor;
}


XMFLOAT4 Light::getDiffuseColor()
{
	return mDiffuseColor;
}


XMFLOAT4 Light::getSpecularColor()
{
	return mSpecularColor;
}


float Light::getSpecularPower()
{
	return mSpecularPower;
}


XMFLOAT3 Light::getDirection()
{
	return mDirection;
}