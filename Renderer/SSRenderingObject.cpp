
#include "SSRendererModulePCH.h"
#include "SSObjectBase.h"
#include "SSRenderingObject.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSShaderManager.h"
#include "SSFreqUsedNames.h"
#include "SSTextureManager.h"
#include "SSSamplerManager.h"
#include "SSCameraManager.h"
#include "SSRenderCommand.h"
#include "SSShader.h"

SSRenderingObject::SSRenderingObject(SSObjectBase* pObject)
	: mpObject(pObject)
{	
	mVertexData = pObject->GetVertexData();
	mMaterialProxy = pObject->GetMaterialProxySharedPtr();
	
	mVertexBuffer = GetDX11Device()->CreateVertexBuffer(mVertexData.Stride, mVertexData.Count, mVertexData.DataPtr);

	if (mVertexData.bHasInstanceData)
	{
		mInstancedDataBuffer = GetDX11Device()->CreateVertexBuffer(	mVertexData.InstanceStride,	mVertexData.InstanceCount,	mVertexData.InstanceDataPtr);
	}
	
	// setup index data
	if(mVertexData.bHasIndexData)
	{	
		mIndexBuffer = GetDX11Device()->CreateIndexBuffer(mVertexData.IndexData);
	}

	CreateRenderCmdList();
}


SSRenderingObject::~SSRenderingObject()
{
	if(mVertexBuffer)
	{	
		mVertexBuffer.reset();
		mVertexBuffer = nullptr;
	}

	if(mIndexBuffer)
	{		
		mIndexBuffer.reset();
		mIndexBuffer = nullptr;
	}


	for (auto* Cmd : RenderCmdList)
	{
		delete Cmd;
	}	
}

void SSRenderingObject::Tick(float delta)
{
	
}

void SSRenderingObject::CreateRenderCmdList()
{
	shared_ptr<SSDX11VertexShader> vs = SSShaderManager::Get().GetVertexShader(mMaterialProxy->GetVertexShaderName());
	shared_ptr<SSDX11PixelShader> ps = SSShaderManager::Get().GetPixelShader(mMaterialProxy->GetPixelShaderName());

	RenderCmdList.push_back(new SSRenderCmdSetVS(vs));
	RenderCmdList.push_back(new SSRenderCmdSetPS(ps));

	if (mVertexData.bHasInstanceData)
	{
		RenderCmdList.push_back(new SSRenderCmdSetInstacedVertexBuffer(mVertexBuffer, mInstancedDataBuffer));		
	}
	else
	{
		RenderCmdList.push_back(new SSRenderCmdSetVertexBuffer(mVertexBuffer));
	}	

	if (mVertexData.bHasIndexData)
	{
		RenderCmdList.push_back(new SSRenderCmdSetIndexBuffer(mIndexBuffer));
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

			RenderCmdList.push_back(new SSRenderCmdUpdateConstantBuffer(ConstantBuffer,k, ConstantBufferData));
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

	// @ set vertex shader texture 
	for (auto& [name, texture] : mMaterialProxy->GetVSTextureMap())
	{
		const int SlotIndex = vs->GetTextureSlotIndex(name);
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(GetDX11Device()->GetDeviceContext(), texture);
		RenderCmdList.push_back(new SSRenderCmdSetVSTexture(vs.get(), resource.get(), SlotIndex));
	}
	
	if (mVertexData.bHasIndexData)
	{
		if (mVertexData.bHasInstanceData)
		{
			RenderCmdList.push_back(new SSRenderCmdDrawIndexedInstanced(mIndexBuffer, mVertexData.InstanceCount));
		}
		else	
		{
			RenderCmdList.push_back(new SSRenderCmdDrawIndexed(mIndexBuffer));
		}
		//
		RenderCmdList.push_back(new SSRenderCmdDrawIndexed(mIndexBuffer));
	}
	else
	{
		RenderCmdList.push_back(new SSRenderCmdDrawWithoutIndex(mVertexBuffer->GetVertexCount()));
	}
}

void SSRenderingObject::Draw(ID3D11DeviceContext* deviceContext)
{
	for (auto* Cmd : RenderCmdList)
	{
		Cmd->Execute(deviceContext);
	}

	return;	
}
