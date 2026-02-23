

#include "SSGLTFRenderingObject.h"
#include "SSDX11StructuredBuffer.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSShaderManager.h"
#include "SSFreqUsedNames.h"
#include "SSTextureManager.h"
#include "SSRenderCommand.h"
#include "SSShader.h"
#include <filesystem>

SSGLTFRenderingObject::SSGLTFRenderingObject(SSObjectBase* InGameObject)
{
	mGLTFMeshObject = static_cast<SSGLTFMeshObject*>(InGameObject);

	std::string GLTFFilePath = mGLTFMeshObject->GetGLTFFilePath();

	std::string BasePath = std::filesystem::path(GLTFFilePath).parent_path().string();

	mGLTFMeshObject->SetScale(5,5,5);

	mMaterialProxy = InGameObject->GetMaterialProxySharedPtr();

	GLTF::SSGLTF_V2& GLTFDataRef = mGLTFMeshObject->mGLTFData;

	mPositionBuffer = new SSDX11StructuredBuffer(GLTFDataRef.MergedPositions.data(), 
		sizeof(XMFLOAT3), GLTFDataRef.MergedPositions.size());

	mNormalBuffer		= new SSDX11StructuredBuffer(GLTFDataRef.MergedNormals.data(), sizeof(XMFLOAT3), GLTFDataRef.MergedNormals.size());
	mTangentBuffer		= new SSDX11StructuredBuffer(GLTFDataRef.MergedTangents.data(), sizeof(XMFLOAT4), GLTFDataRef.MergedTangents.size());
	mTexcoordBuffer		= new SSDX11StructuredBuffer(GLTFDataRef.MergedTexcoords.data(), sizeof(XMFLOAT2), GLTFDataRef.MergedTexcoords.size());

	mIndexBuffer = GetDX11Device()->CreateIndexBuffer(GLTFDataRef.MergedIndices);

	// load all textures 
	for (auto i = 0; i < GLTFDataRef.Textures.size(); ++i)
	{
		GLTF::TextureInfo& TextureInfo = GLTFDataRef.Textures[i];
		std::string TexturePath = BasePath + "/" + TextureInfo.Name;
		std::shared_ptr< SSDX11Texture2D> MatTexture = SSTextureManager::Get().LoadTexture2D(GetDX11Device()->GetDeviceContext(), TexturePath);
		mTextureMap[i] = MatTexture;
	}

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

	std::map<std::string, unsigned int> vsStructuredBufferSlotMap = vs->GetStructuredBufferSlotMap();

	// 
	for (auto& [name, slot] : vsStructuredBufferSlotMap)
	{
		if (name.find("Position") != std::string::npos)
		{
			RenderCmdList.push_back(new SSRenderCmdSetVSShaderResource(mPositionBuffer, slot));
		}
		else if (name.find("Normal") != std::string::npos)
		{
			RenderCmdList.push_back(new SSRenderCmdSetVSShaderResource(mNormalBuffer, slot));
		}
		else if (name.find("Tangent") != std::string::npos)
		{
			RenderCmdList.push_back(new SSRenderCmdSetVSShaderResource(mTangentBuffer, slot));
		}
		else if (name.find("Tex") != std::string::npos)
		{
			RenderCmdList.push_back(new SSRenderCmdSetVSShaderResource(mTexcoordBuffer, slot));
		}
	}

	for (auto& [k, v] : mMaterialProxy->GetVSConstantBufferMap())
	{
		const int SlotIndex = vs->GetConstantBufferSlotIndex(k);
		if (SlotIndex != -1)
		{
			SSConstantBufferData* ConstantBufferData = const_cast<SSConstantBufferData*>(mMaterialProxy->GetVSConstantParam(k));
			check(ConstantBufferData != nullptr);
			SSDX11ConstantBuffer* ConstantBuffer = vs->GetConstantBuffer(k);
			ConstantBuffer->SetBufferData(v);

			RenderCmdList.push_back(new SSRenderCmdUpdateConstantBuffer(ConstantBuffer, k, ConstantBufferData));
			RenderCmdList.push_back(new SSRenderCmdSetVSConstantBuffer(vs.get(), ConstantBuffer, SlotIndex));
		}
	}

	for (auto& [k, v] : mMaterialProxy->GetPSConstantBufferMap())
	{
		const int SlotIndex = ps->GetConstantBufferSlotIndex(k);
		if (SlotIndex != -1)
		{
			SSConstantBufferData* ConstantBufferData = const_cast<SSConstantBufferData*>(mMaterialProxy->GetPSConstantParam(k));
			check(ConstantBufferData != nullptr);
			SSDX11ConstantBuffer* ConstantBuffer = ps->GetConstantBuffer(k);
			ConstantBuffer->SetBufferData(v);

			RenderCmdList.push_back(new SSRenderCmdUpdateConstantBuffer(ConstantBuffer, k, ConstantBufferData));
			RenderCmdList.push_back(new SSRenderCmdSetPSConstantBuffer(ps.get(), ConstantBuffer, SlotIndex));
		}
	}
	// @ set pixel shader texture
	for (auto& [name, texture] : mMaterialProxy->GetPSTextureMap())
	{
		const int SlotIndex = ps->GetTextureSlotIndex(name);
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(GetDX11Device()->GetDeviceContext(), texture);
		RenderCmdList.push_back(new SSRenderCmdSetPSTexture(ps.get(), resource.get(), SlotIndex));
	}	

	RenderCmdList.push_back(new SSSetPrimivitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	GLTF::SSGLTF_V2& GLTFDataRef = mGLTFMeshObject->mGLTFData;

	unsigned int StartIndexLocation = 0;

	unsigned int StartVertexLocation = 0;

	check(GLTFDataRef.IndexCountList.size() == GLTFDataRef.PositionCountList.size());

	
	for (size_t i = 0; i< GLTFDataRef.IndexCountList.size(); ++i)
	{  
		unsigned int IndexCount = GLTFDataRef.IndexCountList[i];
		unsigned int PositionCount = GLTFDataRef.PositionCountList[i];		
		
		RenderCmdList.push_back(new SSRenderCmdDrawIndexed(mIndexBuffer, IndexCount, StartIndexLocation, 0));		
		
		StartIndexLocation += IndexCount;
		StartVertexLocation += PositionCount;
	}
}



