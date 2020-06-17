#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class ModelList
{
private:
	struct ModelInfoType
	{
		XMFLOAT4 color;
		float positionX, positionY, positionZ;
	};

public:
	ModelList();
	ModelList(const ModelList&);
	~ModelList();

	bool initialize(int);
	void shutDown();

	int getModelCount();
	void getData(int, float&, float&, float&, XMFLOAT4&);

private:
	int mModelCount;
	ModelInfoType* mModelInfoList;
};
