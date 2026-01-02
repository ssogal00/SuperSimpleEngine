#pragma once
#include "SSGameModule.h"
#include "SSObjectBase.h"
#include <map>

class GAMEMODULE_API SSGameObjectManager
{
public:
	void AddGameObject(class SSGameObject* object);
	void RemoveGameObject(UINT id);
	void Tick(float delta);

	static SSGameObjectManager& Get();
	static SSGameObjectManager* GetPtr();

	std::map<UINT, SSObjectBase*>& GetGameObjectMap() { return mGameObjectMap; }

	UINT IssueObjectId() { return mNextObjectId++; }

protected:	
	friend class SSGameObject;
	
	UINT mNextObjectId = 1;

	std::map<UINT, SSObjectBase*> mGameObjectMap;
	static SSGameObjectManager* mInstance;
};