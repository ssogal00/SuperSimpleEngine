#pragma once

#include "SSMeshRenderData.h"
#include "SSMaterialProxy.h"
#include "SSObjectBase.h"

class GAMEMODULE_API SSGameObject : public SSObjectBase
{
public:
	SSGameObject();
	virtual ~SSGameObject();

	virtual void OnAddedScene(){}
	virtual void OnRemovedScene(){}
	
	virtual void GameTheadFirstTick(float deltaSeconds){}
	virtual void RenderThreadFirstTick(float deltaSeconds) {}
	
	UINT GetId() const { return mObjectId; }	

protected:

	SSMaterialProxy mMaterialProxy;
};