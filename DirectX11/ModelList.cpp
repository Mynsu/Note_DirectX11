#include "modellist.h"
#include <random>

ModelList::ModelList()
{
	mModelInfoList = 0;
}


ModelList::ModelList(const ModelList& other)
{
}


ModelList::~ModelList()
{
}


bool ModelList::initialize(int numModels)
{
	int i;
	float red, green, blue;

	// Store the number of models.
	mModelCount = numModels;

	// Create a list array of the model information.
	mModelInfoList = new ModelInfoType[mModelCount];
	if(!mModelInfoList)
	{
		return false;
	}

	std::mt19937 rand(std::random_device{}());

	// Go through all the models and randomly generate the model color and position.
	for(i=0; i<mModelCount; i++)
	{
		// Generate a random color for the model.
		constexpr unsigned int RandMax = rand.max();
		red = (float)rand() / RandMax;
		green = (float)rand() / RandMax;
		blue = (float)rand() / RandMax;
		
		mModelInfoList[i].color = XMFLOAT4(red, green, blue, 1.0f);

		// Generate a random position in front of the viewer for the mode.
		mModelInfoList[i].positionX = (((float)rand()-(float)rand())/RandMax) * 10.0f;
		mModelInfoList[i].positionY = (((float)rand()-(float)rand())/RandMax) * 10.0f;
		mModelInfoList[i].positionZ = ((((float)rand()-(float)rand())/RandMax) * 10.0f) + 5.0f;
	}

	return true;
}

void ModelList::shutDown()
{
	// Release the model information list.
	if(mModelInfoList)
	{
		delete [] mModelInfoList;
		mModelInfoList = 0;
	}

	return;
}

int ModelList::getModelCount()
{
	return mModelCount;
}

void ModelList::getData(int index, float& positionX, float& positionY, float& positionZ, XMFLOAT4& color)
{
	positionX = mModelInfoList[index].positionX;
	positionY = mModelInfoList[index].positionY;
	positionZ = mModelInfoList[index].positionZ;

	color = mModelInfoList[index].color;

	return;
}