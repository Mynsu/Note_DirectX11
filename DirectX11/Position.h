#pragma once

class Position
{
public:
	Position();
	Position(const Position&);
	~Position();

	void setFrameTime(float);
	void getRotation(float&);

	void turnLeft(bool);
	void turnRight(bool);

private:
	float mFrameTime;
	float mRotationY;
	float mLeftTurnSpeed, mRightTurnSpeed;
};