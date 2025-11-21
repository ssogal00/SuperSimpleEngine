#pragma once

#include <memory>
#include "SSMeshRenderData.h"

class CORE_API SSObjectBase
{
public:
	SSObjectBase();
	virtual ~SSObjectBase();

	virtual void Destroy() {}	
	virtual void Draw(ID3D11DeviceContext* deviceContext) {}
	
	virtual const SSMeshRenderData& GetRenderData();
	virtual const SSMeshVertexIndexData& GetVertexData();
	virtual std::vector<SSMeshElementRenderData> GetMeshElementRenderData();

	virtual void Tick(float delta) {}

	virtual DirectX::XMMATRIX GetModelTransform();

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

	void SetVertexShader(SSName vs);
	void SetPixelShader(SSName ps);

	void SetPSTexture(SSName name, SSName textureName);
	void SetVSTexture(SSName name , SSName textureName);

	void SetVSConstantBufferData(SSName name, class SSConatantBufferData&& buffer);
	void SetPSConstantBufferData(SSName name, class SSConatantBufferData&& buffer);

protected:
	virtual void CreateRenderData() {}
	virtual void CreateVertexData() {}
	virtual void CreateInstanceData() {}

	SSMeshVertexIndexData mVertexData;
	SSMeshRenderData mRenderData;	

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