#include "position.h"

Position::Position()
{
	mFrameTime = 0.0f;
	mRotationY = 0.0f;
	mLeftTurnSpeed  = 0.0f;
	mRightTurnSpeed = 0.0f;
}


Position::Position(const Position& other)
{
}


Position::~Position()
{
}

void Position::setFrameTime(float time)
{
	mFrameTime = time;
	return;
}

void Position::getRotation(float& y)
{
	y = mRotationY;
	return;
}

void Position::turnLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if(keydown)
	{
		mLeftTurnSpeed += mFrameTime * 0.01f;

		if(mLeftTurnSpeed > (mFrameTime * 0.15f))
		{
			mLeftTurnSpeed = mFrameTime * 0.15f;
		}
	}
	else
	{
		mLeftTurnSpeed -= mFrameTime* 0.005f;

		if(mLeftTurnSpeed < 0.0f)
		{
			mLeftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	mRotationY -= mLeftTurnSpeed;
	if(mRotationY < 0.0f)
	{
		mRotationY += 360.0f;
	}

	return;
}


void Position::turnRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if(keydown)
	{
		mRightTurnSpeed += mFrameTime * 0.01f;

		if(mRightTurnSpeed > (mFrameTime * 0.15f))
		{
			mRightTurnSpeed = mFrameTime * 0.15f;
		}
	}
	else
	{
		mRightTurnSpeed -= mFrameTime* 0.005f;

		if(mRightTurnSpeed < 0.0f)
		{
			mRightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	mRotationY += mRightTurnSpeed;
	if(mRotationY > 360.0f)
	{
		mRotationY -= 360.0f;
	}

	return;
}