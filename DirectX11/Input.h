#pragma once
#include <bitset>

const unsigned int MAX_KEYS = 256;

class Input
{
public:
	Input( ) = default;
	Input( const Input& ) = delete;
	~Input( ) = default;

	void keyDown( const unsigned int input )
	{
		mKeys.set( input );
	}
	void keyUp( const unsigned int input )
	{
		mKeys.reset( input );
	}
	bool isKeyDown( const unsigned int key )
	{
		return mKeys.test( key );
	}
private:
	std::bitset<MAX_KEYS> mKeys;
};
