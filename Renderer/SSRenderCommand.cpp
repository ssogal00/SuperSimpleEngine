
#include "SSRenderCommand.h"
#include "SSShader.h"
#include "SSDX11Texture2D.h"
#include "SSDX11RenderTarget.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSMemoryAllocator.h"

SSRenderCmdBase::~SSRenderCmdBase()
{	
}

void SSRenderCmdSetVS::Execute(ID3D11DeviceContext * inDeviceContext)
{
	if (GetDX11Device()->GetBoundVertexShader() == mVS->GetShader())
	{
		return;
	}
	D3D_PRIMITIVE_TOPOLOGY PrimitiveType = mVS->GetPrimitiveType();
	inDeviceContext->IAGetPrimitiveTopology(&PrimitiveType);
	inDeviceContext->IASetInputLayout(mVS->GetInputLayout());
	inDeviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
}

SSRenderCmdSetInstacedVertexBuffer::SSRenderCmdSetInstacedVertexBuffer(
	std::shared_ptr<SSDX11VertexBuffer> inVB,
	std::shared_ptr<SSDX11VertexBuffer> inInstanceVB)
	: mVertexBuffer(inVB), mInstanceBuffer(inInstanceVB)
{
}

void SSRenderCmdSetInstacedVertexBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	UINT strides[2] = { mVertexBuffer->GetStride(), mInstanceBuffer->GetStride() };
	
	UINT offsets[2] = { 0,0 };
	
	ID3D11Buffer* buffers[2] = 
	{
		(ID3D11Buffer*)mVertexBuffer->GetBufferPointer(),
		(ID3D11Buffer*)mInstanceBuffer->GetBufferPointer()
	};

	inDeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
}

void* SSRenderCmdSetVS::operator new(size_t size)
{
	return SSMemoryManager::Get().Alloc(size);
}

void SSRenderCmdSetVS::operator delete(void* Ptr)
{
	SSMemoryManager::Get().DeAlloc(Ptr, sizeof(SSRenderCmdSetVS));
}

void SSRenderCmdSetPS::Execute(ID3D11DeviceContext* inDeviceContext)
{
	ID3D11PixelShader* BoundShader = GetDX11Device()->GetBoundPixelShader();
	if (BoundShader == mPS->GetShader())
	{
		return;
	}
	inDeviceContext->PSSetShader(mPS->GetShader(), nullptr, 0);
}

SSRenderCmdSetVSTexture::SSRenderCmdSetVSTexture(SSDX11VertexShader* inVS, SSDX11Texture2D * inTex, unsigned int slotIndex)
	:mVS(inVS), mTex(inTex),mSlotIndex(slotIndex)
{
}

void SSRenderCmdSetVSTexture::Execute(ID3D11DeviceContext* inDeviceContext)
{
	ID3D11ShaderResourceView* BoundShaderResourceView = nullptr;
	inDeviceContext->VSGetShaderResources(mSlotIndex, 1, &BoundShaderResourceView);
	if (BoundShaderResourceView == mTex->GetShaderResourceView())
	{
		return;
	}
	inDeviceContext->VSSetShaderResources(mSlotIndex, 1, mTex->GetShaderResourceViewRef());
}

SSRenderCmdDrawIndexedInstanced::SSRenderCmdDrawIndexedInstanced(std::shared_ptr<SSDX11IndexBuffer> inBuffer, unsigned int InInstanceCount)
	: mIndexBuffer(inBuffer), mInstanceCount(InInstanceCount)
{
}

void SSRenderCmdDrawIndexedInstanced::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->IASetIndexBuffer((ID3D11Buffer*)mIndexBuffer->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);
	inDeviceContext->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), mInstanceCount, 0, 0, 0);
}

SSRenderCmdDrawWithoutIndexInstanced::SSRenderCmdDrawWithoutIndexInstanced(std::shared_ptr<SSDX11VertexBuffer> inInstanceVB,  
	unsigned int InVertexCount, unsigned int InInstanceCount)
	: mInstanceBuffer(inInstanceVB), mVertexCount(InVertexCount), mInstanceCount(InInstanceCount)
{	
}

void SSRenderCmdDrawWithoutIndexInstanced::Execute(ID3D11DeviceContext* inDeviceContext)
{
	auto stride = mInstanceBuffer->GetStride();
	UINT offset = 0;
	check(mVertexCount > 0);
	check(mInstanceCount > 0);
	inDeviceContext->IASetVertexBuffers(1, 1, (ID3D11Buffer* const*)mInstanceBuffer->GetBufferPointerRef(), &stride, &offset);
	inDeviceContext->DrawInstanced(mVertexCount, mInstanceCount, 0, 0);
}

SSRenderCmdSetPSTexture::SSRenderCmdSetPSTexture(class SSDX11PixelShader* inPS, class SSDX11Texture2D* inTex, unsigned int slotIndex)
	: mPS(inPS), mTex(inTex), mSlotIndex(slotIndex)
{
}

void SSRenderCmdSetPSTexture::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->PSSetShaderResources(mSlotIndex, 1, mTex->GetShaderResourceViewRef());
}

SSRenderCmdSetVSConstantBuffer::SSRenderCmdSetVSConstantBuffer(SSDX11VertexShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot)	
	: mVS(inVS), mBuffer(inBuffer),mSlotIndex(slot)
{
}

void SSRenderCmdSetVSConstantBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	ID3D11Buffer* boundBuffer = GetDX11Device()->GetBoundConstantBufferVS(mSlotIndex);
	if (boundBuffer == mBuffer->GetDX11BufferPointer())
	{
		return;
	}
	inDeviceContext->VSSetConstantBuffers(mSlotIndex, 1, (ID3D11Buffer* const*) mBuffer->GetBufferPointerRef());
}

SSRenderCmdSetPSConstantBuffer::SSRenderCmdSetPSConstantBuffer(SSDX11PixelShader* inPS, class SSDX11Buffer* inBuffer, unsigned int slot)
	: mPS(inPS), mBuffer(inBuffer), mSlotIndex(slot)
{
}

void SSRenderCmdSetPSConstantBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	ID3D11Buffer* boundBuffer = GetDX11Device()->GetBoundConstantBufferPS(mSlotIndex);
	if (boundBuffer == mBuffer->GetDX11BufferPointer())
	{
		return;
	}
	inDeviceContext->PSSetConstantBuffers(mSlotIndex, 1, (ID3D11Buffer* const*)mBuffer->GetBufferPointerRef());
}

SSRenderCmdUpdateConstantBuffer::SSRenderCmdUpdateConstantBuffer(SSDX11Buffer* ptrBuffer, std::string InBufferName, SSConstantBufferData* InBufferData)
	: mBuffer(ptrBuffer), mBufferName(InBufferName), mBufferData(InBufferData)
{
	
}

void SSRenderCmdUpdateConstantBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HR(inDeviceContext->Map(mBuffer->GetDX11BufferPointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	
	memcpy_s(mappedResource.pData, mBuffer->GetBufferSize(), mBufferData->GetData(), mBuffer->GetBufferSize());
	
	inDeviceContext->Unmap(mBuffer->GetDX11BufferPointer(), 0);
}


SSRenderCmdSetRenderTarget::SSRenderCmdSetRenderTarget(class SSDX11RenderTarget* inRT)
	: mRenderTarget(inRT)
{
}

void SSRenderCmdSetRenderTarget::Execute(ID3D11DeviceContext* inDeviceContext)
{	
	if (mRenderTarget)
	{
		ID3D11RenderTargetView* viewArray[SSDX11RenderTarget::MAX_COUNT]{ nullptr };
		ID3D11DepthStencilView* depthStencilView{ nullptr };

		const unsigned int nCount = mRenderTarget->GetCount();

		for (auto i = 0u; i < nCount; ++i)
		{
			viewArray[i] = mRenderTarget->GetRenderTargetView(nCount);
		}

		if (mRenderTarget->IsDepthExist())
		{
			depthStencilView = mRenderTarget->GetDepthStencilView();
		}

		inDeviceContext->OMSetRenderTargets(nCount, viewArray, depthStencilView); 
	}
}


SSRenderCmdDrawIndexed::SSRenderCmdDrawIndexed(std::shared_ptr<SSDX11IndexBuffer> inBuffer)
	: mIndexBuffer(inBuffer)
{
}

void SSRenderCmdDrawIndexed::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->IASetIndexBuffer((ID3D11Buffer*)mIndexBuffer->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);
	inDeviceContext->DrawIndexed(mIndexBuffer->GetIndexCount(), 0, 0);
}

SSRenderCmdSetVertexBuffer::SSRenderCmdSetVertexBuffer(std::shared_ptr<SSDX11VertexBuffer> inVB, unsigned int inOffset)
	:mVertexBuffer(inVB), mOffset(inOffset)
{
}

void SSRenderCmdSetVertexBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{	
	auto stride = mVertexBuffer->GetStride();
	UINT offset = mOffset;
	inDeviceContext->IASetVertexBuffers(0, 1, (ID3D11Buffer* const*)mVertexBuffer->GetBufferPointerRef(), &stride, &offset);
}

SSRenderCmdSetIndexBuffer::SSRenderCmdSetIndexBuffer(std::shared_ptr<SSDX11IndexBuffer> inIB, unsigned int inOffset)
	: mIndexBuffer(inIB),mOffset(inOffset)
{
}

void SSRenderCmdSetIndexBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->IASetIndexBuffer((ID3D11Buffer*)mIndexBuffer->GetBufferPointer(), DXGI_FORMAT_R32_UINT, mOffset);
}

SSRenderCmdDrawWithoutIndex::SSRenderCmdDrawWithoutIndex(unsigned int InVertexCount)
	:mVertexCount(InVertexCount)
{
}

void SSRenderCmdDrawWithoutIndex::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->Draw(mVertexCount, 0);
}


void SSRenderCmdSetDepthStencilState::Execute(ID3D11DeviceContext* inDeviceContext)
{
	//inDeviceContext->OMSetDepthStencilState();
}

SSRenderCmdSetVSShaderResource::SSRenderCmdSetVSShaderResource(SSDX11StructuredBuffer* inBuffer, unsigned int slotIndex)
	: mBuffer(inBuffer), mSlotIndex(slotIndex)
{
}

void SSRenderCmdSetVSShaderResource::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->VSSetShaderResources(mSlotIndex, 1, mBuffer->GetSRVPointerRef());
}