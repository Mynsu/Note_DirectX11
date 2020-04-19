#pragma once

#include <DirectXMath.h>

class Light
{
public:
	Light() = default;
	Light( const Light& ) = delete;
	~Light() = default;

	void setDiffuseColor( float red, float green, float blue, float alpha )
	{
		mDiffuseColor = DirectX::XMFLOAT4(red, green, blue, alpha);
	}
	void setPosition( float x, float y, float z )
	{
		mPosition = DirectX::XMFLOAT3(x, y, z);
	}
	DirectX::XMFLOAT4 getDiffuseColor()
	{
		return mDiffuseColor;
	}
	DirectX::XMFLOAT3 getPosition()
	{
		return mPosition;
	}
private:
	DirectX::XMFLOAT4 mDiffuseColor;
	DirectX::XMFLOAT3 mPosition;
};