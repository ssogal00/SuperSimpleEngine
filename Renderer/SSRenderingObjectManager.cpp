#include "SSRenderingObjectManager.h"
#include "SSRenderingObject.h"
#include "SSGameObjectManager.h"
#include "SSRenderingObjectFactory.h"

SSRenderingObjectManager::SSRenderingObjectManager()
{
	InitializeCriticalSection(&mCriticalSection);
}

void SSRenderingObjectManager::Initialize()
{
}

void SSRenderingObjectManager::Shutdown()
{
	DeleteAllObjects();
}

void SSRenderingObjectManager::Tick(float deltaTime)
{
	for (auto& [k, v] : mRenderingObjectMap)
	{
		v->Tick(deltaTime);
	}
}

// called from rendering thread
void SSRenderingObjectManager::UpdateObjects()
{
	EnterCriticalSection(&mCriticalSection);

	auto& GameObjects = SSGameObjectManager::Get().GetGameObjectMap();
	
	for (auto& [k, v] : GameObjects)
	{
		if (mRenderingObjectMap.count(k) == 0)
		{
			//mRenderingObjectMap[k] = new SSRenderingObject(v);
			mRenderingObjectMap[k] = SSRenderingObjectFactory::CreateRenderingObject(v);
		}
	}
	
	std::vector<int> ObjectsToDelete;

	for (auto& [k, v] : mRenderingObjectMap)
	{
		if (GameObjects.count(k) == 0)
		{
			ObjectsToDelete.push_back(k);
		}
	}

	for (int Id : ObjectsToDelete)
	{
		SSRenderingObject* Object = mRenderingObjectMap[Id];
		
		delete Object;

		mRenderingObjectMap.erase(Id);
	}

	LeaveCriticalSection(&mCriticalSection);
}

void SSRenderingObjectManager::DeleteAllObjects()
{
	EnterCriticalSection(&mCriticalSection);

	for (auto& [k, v] : mRenderingObjectMap)
	{
		delete mRenderingObjectMap[k];
	}

	mRenderingObjectMap.clear();

	LeaveCriticalSection(&mCriticalSection);
}

// called from game thread
void SSRenderingObjectManager::SetPendingObjects(std::map<UINT, SSObjectBase*> objectMap)
{
	EnterCriticalSection(&mCriticalSection);
	mPendingObjectMap = objectMap;
	LeaveCriticalSection(&mCriticalSection);
}



