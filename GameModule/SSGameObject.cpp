
#include "SSGameModule.h"
#include "SSGameObject.h"
#include "SSMeshRenderData.h"
#include "SSGameObjectManager.h"
#include "SSCameraManager.h"

using namespace DirectX;

SSGameObject::SSGameObject()
{
	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
	
	mObjectId = SSGameObjectManager::Get().IssueObjectId();
	
	SSGameObjectManager::Get().AddGameObject(this);	

	mMaterialProxy.SetVSConstantParam("Model", mModelCBufferData);
	mMaterialProxy.SetVSConstantParam("View", mViewCBufferData);
	mMaterialProxy.SetVSConstantParam("Proj", mProjCBufferData);
}

SSGameObject::~SSGameObject()
{
	SSGameObjectManager::Get().RemoveGameObject(mObjectId);
	mObjectId = 0;
}

void SSGameObject::Tick(float fDeltaSeconds)
{
	SSObjectBase::Tick(fDeltaSeconds);

	mModelCBufferData.SetBufferData(XMMatrixTranspose(GetModelTransform()));
	mMaterialProxy.SetVSConstantParam("Model", mModelCBufferData);

	mViewCBufferData.SetBufferData(XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	mMaterialProxy.SetVSConstantParam("View", mViewCBufferData);

	mProjCBufferData.SetBufferData(XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));
	mMaterialProxy.SetVSConstantParam("Proj", mProjCBufferData);
}
