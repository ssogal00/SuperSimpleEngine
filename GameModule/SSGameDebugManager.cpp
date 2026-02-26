

#include "SSGameDebugManager.h"


SSGameDebugManager& SSGameDebugManager::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new SSGameDebugManager();
	}
	return *mInstance;
}


SSGameDebugManager* SSGameDebugManager::mInstance = nullptr;