#pragma once

#include <memory>
#include "SSMeshRenderData.h"

class SSRenderingObject
{
public:
	SSRenderingObject(class SSObjectBase* pObject);
	SSRenderingObject(){}
	
	virtual ~SSRenderingObject();
	virtual void Draw(ID3D11DeviceContext* deviceContext);

	bool IsTranslucent() const { return bIsTranslucent; }

	virtual void Tick(float delta);

protected:

	XMMATRIX ModelMatrix;

	virtual void CreateRenderCmdList();
	std::vector<class SSRenderCmdBase*> RenderCmdList;

	SSObjectBase* mpObject = nullptr;

	SSMeshRenderData mRenderData;
	SSMeshVertexIndexData mVertexData;

	std::shared_ptr<class SSDX11VertexBuffer> mVertexBuffer = nullptr;
	std::shared_ptr<class SSDX11InstancedVertexBuffer> mInstancedVertexBuffer = nullptr;
	std::shared_ptr<class SSDX11IndexBuffer> mIndexBuffer = nullptr;

	std::shared_ptr<class SSDX11VertexShader> mVS = nullptr;
	std::shared_ptr<class SSDX11PixelShader> mPS = nullptr;

	bool bIsTranslucent	 = false;

	class SSMaterial* mMaterial = nullptr;
};