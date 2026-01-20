

#include "SSGLTFRenderingObject.h"
#include "SSDX11StructuredBuffer.h"

SSGLTFRenderingObject::SSGLTFRenderingObject(SSObjectBase* InGameObject)
{
	mGLTFMeshObject = static_cast<SSGLTFMeshObject*>(InGameObject);

	mPositionBuffer = new SSDX11StructuredBuffer(mMeshVertexData.PositionList.data(), sizeof(XMFLOAT3), mMeshVertexData.PositionList.size());
	mNormalBuffer = new SSDX11StructuredBuffer(mMeshVertexData.NormalList.data(), sizeof(XMFLOAT3), mMeshVertexData.NormalList.size());
	mTangentBuffer = new SSDX11StructuredBuffer(mMeshVertexData.TangentList.data(), sizeof(XMFLOAT3), mMeshVertexData.TangentList.size());
	mTexcoordBuffer = new SSDX11StructuredBuffer(mMeshVertexData.TexcoordList.data(), sizeof(XMFLOAT2), mMeshVertexData.TexcoordList.size());
}

SSGLTFRenderingObject::~SSGLTFRenderingObject()
{
	
}

void SSGLTFRenderingObject::CreateRenderCmdList()
{


}