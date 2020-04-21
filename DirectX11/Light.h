#pragma once

#include <DirectXMath.h>

class Light
{
public:
	Light() = default;
	Light( const Light& ) = delete;
	~Light() = default;

	void setAmbientColor( float red, float green, float blue, float alpha )
	{
		mAmbientColor = DirectX::XMFLOAT4(red, green, blue, alpha);
	}
	void setDiffuseColor( float red, float green, float blue, float alpha )
	{
		mDiffuseColor = DirectX::XMFLOAT4(red, green, blue, alpha);
	}
	void setPosition( float x, float y, float z )
	{
		mPosition = DirectX::XMFLOAT3(x, y, z);
	}
	void setSpecularColor( float red, float green, float blue, float alpha )
	{
		mSpecularColor = DirectX::XMFLOAT4(red, green, blue, alpha);
	}
	void setSpecularPower( float power )
	{
		mSpecularPower = power;
	}
	DirectX::XMFLOAT4 getAmbientColor()
	{
		return mAmbientColor;
	}
	DirectX::XMFLOAT4 getDiffuseColor()
	{
		return mDiffuseColor;
	}
	DirectX::XMFLOAT3 getPosition()
	{
		return mPosition;
	}
	DirectX::XMFLOAT4 getSpecularColor()
	{
		return mSpecularColor;
	}
	float getSpecularPower()
	{
		return mSpecularPower;
	}
private:
	DirectX::XMFLOAT4 mAmbientColor;
	DirectX::XMFLOAT4 mDiffuseColor;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT4 mSpecularColor;
	float mSpecularPower;
};