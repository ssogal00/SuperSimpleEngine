

#include "SSGLTFRenderingObject.h"
#include "SSDX11StructuredBuffer.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSShaderManager.h"
#include "SSFreqUsedNames.h"
#include "SSTextureManager.h"
#include "SSRenderCommand.h"

SSGLTFRenderingObject::SSGLTFRenderingObject(SSObjectBase* InGameObject)
{
	mGLTFMeshObject = static_cast<SSGLTFMeshObject*>(InGameObject);
	
	if (mMeshVertexData.PositionList.size() > 0)
	{
		mPositionBuffer = new SSDX11StructuredBuffer(mMeshVertexData.PositionList.data(), sizeof(XMFLOAT3), mMeshVertexData.PositionList.size());
	}
	
	mNormalBuffer		= new SSDX11StructuredBuffer(mMeshVertexData.NormalList.data(), sizeof(XMFLOAT3), mMeshVertexData.NormalList.size());
	mTangentBuffer		= new SSDX11StructuredBuffer(mMeshVertexData.TangentList.data(), sizeof(XMFLOAT3), mMeshVertexData.TangentList.size());
	mTexcoordBuffer		= new SSDX11StructuredBuffer(mMeshVertexData.TexcoordList.data(), sizeof(XMFLOAT2), mMeshVertexData.TexcoordList.size());

	CreateRenderCmdList();
}

SSGLTFRenderingObject::~SSGLTFRenderingObject()
{
	if (mPositionBuffer)
	{
		delete mPositionBuffer;
	}
	if(mNormalBuffer)
	{
		delete mNormalBuffer;
	}
	if(mTangentBuffer)
	{
		delete mTangentBuffer;
	}
	if(mTexcoordBuffer)
	{
		delete mTexcoordBuffer;
	}
}

void SSGLTFRenderingObject::CreateRenderCmdList()
{
	std::shared_ptr<SSDX11VertexShader> vs = SSShaderManager::Get().GetVertexShader(mMaterialProxy->GetVertexShaderName());
	std::shared_ptr<SSDX11PixelShader> ps = SSShaderManager::Get().GetPixelShader(mMaterialProxy->GetPixelShaderName());

	RenderCmdList.push_back(new SSRenderCmdSetVS(vs));
	RenderCmdList.push_back(new SSRenderCmdSetPS(ps));

}