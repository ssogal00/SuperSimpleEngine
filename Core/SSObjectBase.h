#pragma once

#include <memory>
#include "SSMeshRenderData.h"
#include "SSMaterialProxy.h"

class CORE_API SSObjectBase
{
public:
	SSObjectBase();
	virtual ~SSObjectBase();

	virtual void Destroy() {}	
	virtual void Draw(ID3D11DeviceContext* deviceContext) {}
	
	
	virtual std::shared_ptr<SSMaterialProxy> GetMaterialProxySharedPtr();
	virtual const SSMeshVertexIndexData& GetVertexData();

	virtual void Tick(float delta) {}

	virtual DirectX::XMMATRIX GetModelTransform();

	//virtual float GetDistanceFromCamera() const;

	UINT GetId() const { return mObjectId; }

	virtual void SetPosition(float x, float y, float z);
	virtual void SetScale(float x, float y, float z);

	virtual void SetPositionX(float x);
	virtual void SetPositionY(float y);
	virtual void SetPositionZ(float z);

	virtual void SetScaleX(float x);
	virtual void SetScaleY(float y);
	virtual void SetScaleZ(float z);

	bool IsVisible() const { return mVisible; }

	bool IsInstanced() const { return false; }

	void SetInstancedData(const SSInstancedVertexData& instanceData) {}

protected:
	virtual void CreateRenderData() {}
	virtual void CreateVertexData() {}
	virtual void CreateInstanceData() {}

	SSMeshVertexIndexData mVertexData;
	SSMeshRenderData mRenderData;
	std::shared_ptr<SSMaterialProxy> mMaterialProxy;

	friend class SSGameObjectManager;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mScale;

	bool mVisible = true;
	
	float mYaw = 0;
	float mPitch = 0;
	float mRoll = 0;
	UINT mObjectId = 0;
	
	std::shared_ptr<class SSMaterial> mMaterial;
	
};