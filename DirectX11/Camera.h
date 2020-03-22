#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	Camera( )
		: mPositionX( 0.f ), mPositionY( 0.f ), mPositionZ( 0.f ),
		mRotationX( 0.f ), mRotationY( 0.f ), mRotationZ( 0.f )
	{}
	Camera( const Camera& ) = delete;
	~Camera( ) = default;

	void setPosition( const float x, const float y, const float z )
	{
		mPositionX = x;
		mPositionY = y;
		mPositionZ = z;
	}
	void setRotation( const float x, const float y, const float z )
	{
		mRotationX = x;
		mRotationY = y;
		mRotationZ = z;
	}
	DirectX::XMFLOAT3 getPosition( )
	{
		return DirectX::XMFLOAT3(mPositionX, mPositionY, mPositionZ);
	}
	DirectX::XMFLOAT3 getRotation( )
	{
		return DirectX::XMFLOAT3(mRotationX, mRotationY, mRotationZ);
	}
	void render( );
	void getViewMatrix( DirectX::XMMATRIX& viewMatrix )
	{
		viewMatrix = mViewMatrix;
	}
private:
	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;
	DirectX::XMMATRIX mViewMatrix;
};
