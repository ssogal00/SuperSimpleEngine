
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
	mRenderData = pObject->GetRenderData();
	mVertexData = pObject->GetVertexData();
	mMaterialProxy = pObject->GetMaterialProxy();
	
	mVertexBuffer = GetDX11Device()->CreateVertexBuffer(mVertexData.Stride, mVertexData.Count, mVertexData.DataPtr);
	
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
	shared_ptr<SSDX11VertexShader> vs = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);

	if (vs)
	{
		mMaterialProxy = mpObject->GetMaterialProxy();
		for(auto& [k, v] : mMaterialProxy.GetVSConstantBufferMap())
		{
			const int SlotIndex = vs->GetConstantBufferSlotIndex(k);
			if (SlotIndex != -1)
			{
				const SSConstantBufferData* ConstantBufferData = mMaterialProxy.GetVSConstantParam(k);
				BYTE* DataPtr = ConstantBufferData->GetData();
				SSDX11ConstantBuffer* ConstantBuffer = vs->GetConstantBuffer(k);
				ConstantBuffer->UpdateBufferData(DataPtr, ConstantBufferData->GetBufferSize());
			}
		}

		/*const int SlotIndex = vs->GetConstantBufferSlotIndex("Model");
		if (SlotIndex != -1)
		{
			const SSConstantBufferData* Data2 = mMaterialProxy.GetVSConstantParam("Model");
			XMMATRIX Model2Matrix = *Data2->GetDataAs<XMMATRIX>();
			SSDX11ConstantBuffer* ConstantBuffer = vs->GetConstantBuffer("Model");
			ConstantBuffer->UpdateBufferData(&Model2Matrix, sizeof(XMMATRIX));
		}
		*/
	}
}

void SSRenderingObject::CreateRenderCmdList()
{
	shared_ptr<SSDX11VertexShader> vs = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);
	shared_ptr<SSDX11PixelShader> ps = SSShaderManager::Get().GetPixelShader(mRenderData.PixelShaderName);
	
	RenderCmdList.push_back(new SSRenderCmdSetVS(vs));
	RenderCmdList.push_back(new SSRenderCmdSetPS(ps));	
	RenderCmdList.push_back(new SSRenderCmdSetVertexBuffer(mVertexBuffer));

	if (mVertexData.bHasIndexData)
	{
		RenderCmdList.push_back(new SSRenderCmdSetIndexBuffer(mIndexBuffer));
	}

	for (auto& [k, v] : mMaterialProxy.GetVSConstantBufferMap())
	{
		const int SlotIndex = vs->GetConstantBufferSlotIndex(k);
		if (SlotIndex != -1)
		{
			SSDX11ConstantBuffer* ConstantBuffer = vs->GetConstantBuffer(k);
			ConstantBuffer->SetBufferData(v);

			RenderCmdList.push_back(new SSRenderCmdUpdateConstantBuffer(ConstantBuffer,k));
			RenderCmdList.push_back(new SSRenderCmdSetVSConstantBuffer(vs.get(), ConstantBuffer, SlotIndex));			
		}
	}

	// set vertex shader constants
	for (auto& [k, v] : mRenderData.VSConstantBufferMap)
	{		
		const int SlotIndex = vs->GetConstantBufferSlotIndex(k);
		if (SlotIndex != -1)
		{
			SSDX11ConstantBuffer* ConstantBuffer = vs->GetConstantBuffer(k);
			ConstantBuffer->SetBufferData(v);			

			RenderCmdList.push_back(new SSRenderCmdSetVSConstantBuffer(vs.get(), ConstantBuffer, SlotIndex));
			RenderCmdList.push_back(new SSRenderCmdUpdateConstantBuffer(ConstantBuffer,k));
		}
	}

	// set pixel shader constants
	for (auto& [k, v] : mRenderData.PSConstantBufferMap)
	{		
		const int SlotIndex = ps->GetConstantBufferSlotIndex(k);
		if (SlotIndex != -1)
		{
			SSDX11ConstantBuffer* ConstantBuffer = ps->GetConstantBuffer(k);
			ConstantBuffer->SetBufferData(v);

			RenderCmdList.push_back(new SSRenderCmdSetPSConstantBuffer(ps.get(), ps->GetConstantBuffer(k), SlotIndex));
			RenderCmdList.push_back(new SSRenderCmdUpdateConstantBuffer(ConstantBuffer,k));
		}
	}	

	// @ set pixel shader texture
	for (auto& [name, texture] : mRenderData.PSTextureMap)
	{
		const int SlotIndex = ps->GetTextureSlotIndex(name);
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(GetDX11Device()->GetDeviceContext(), texture);
		RenderCmdList.push_back(new SSRenderCmdSetPSTexture(ps.get(), resource.get(), SlotIndex));
	}

	// @ set vertex shader texture 
	for (auto& [name, texture] : mRenderData.VSTextureMap)
	{
		const int SlotIndex = vs->GetTextureSlotIndex(name);
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(GetDX11Device()->GetDeviceContext(), texture);
		RenderCmdList.push_back(new SSRenderCmdSetVSTexture(vs.get(), resource.get(), SlotIndex));
	}

	if (mVertexData.bHasIndexData)
	{
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
