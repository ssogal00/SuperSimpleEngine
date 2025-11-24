#pragma once

#include "SSMeshRenderData.h"
#include "SSMaterialProxy.h"
#include "SSObjectBase.h"
#include "SSMathHelper.h"

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

	virtual void Tick(float deltaSeconds) override;

protected:

	SSConstantBufferData mModelCBufferData{ SSMathHelper::IdentityMatrix4X4 };
	SSConstantBufferData mViewCBufferData{ SSMathHelper::IdentityMatrix4X4 };
	SSConstantBufferData mProjCBufferData{ SSMathHelper::IdentityMatrix4X4 };
};