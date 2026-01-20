#pragma once

#include "SSRendererModulePCH.h"
#include "SSGLTFMesh.h"
#include "SSRenderingObject.h"

class SSDX11StructuredBuffer;

class SSGLTFRenderingObject : public SSRenderingObject
{
public:
	SSGLTFRenderingObject(SSObjectBase* InGameObject);
	~SSGLTFRenderingObject();
protected:
	virtual void CreateRenderCmdList() override;
private:
	class SSGLTFMeshObject* mGLTFMeshObject = nullptr;
	SSManualFetchMeshVertexData mMeshVertexData;

	//
	SSDX11StructuredBuffer* mPositionBuffer = nullptr;
	SSDX11StructuredBuffer* mNormalBuffer = nullptr;
	SSDX11StructuredBuffer* mTangentBuffer = nullptr;
	SSDX11StructuredBuffer* mTexcoordBuffer = nullptr;
};
