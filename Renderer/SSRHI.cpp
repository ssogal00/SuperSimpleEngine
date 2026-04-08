
#include "SSRHI.h"
#include "SSDX11IndexBuffer.h"
#include "SSDX11Renderer.h"
#include "SSDX11ConstantBuffer.h"
#include "SSShader.h"
#include "SSDX11Texture2D.h"

std::shared_ptr<SSDX11IndexBuffer> RHICreateIndexBuffer(std::vector<unsigned short>& InData)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(unsigned short);
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * InData.size());

	D3D11_SUBRESOURCE_DATA indexSubresourceData;
	indexSubresourceData.pSysMem = &InData[0];
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* ptrBuffer = nullptr;

	HR(GetDX11DevicePtr()->CreateBuffer(&bufferDesc, &indexSubresourceData, &ptrBuffer));

	std::shared_ptr<SSDX11IndexBuffer> Result = std::make_shared<SSDX11IndexBuffer>(ptrBuffer, static_cast<unsigned int>(InData.size()));

	return Result;
}


std::shared_ptr<SSDX11VertexBuffer> RHICreateVertexBuffer(unsigned int InStride, unsigned int InCount, void* InPtrData, bool bIsImmutable)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = bIsImmutable ? 0 : D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage = bIsImmutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = static_cast<UINT>(InStride * InCount);

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = InPtrData;
	vertexSubresourceData.SysMemPitch = 0;
	vertexSubresourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* PtrBuffer = nullptr;
	HR(GetDX11DevicePtr()->CreateBuffer(&bufferDesc, &vertexSubresourceData, &PtrBuffer));
	std::shared_ptr<SSDX11VertexBuffer> Result = std::make_shared<SSDX11VertexBuffer>(PtrBuffer, InStride, InCount);
	return Result;
}

std::shared_ptr<SSDX11ConstantBuffer> RHICreateConstantBuffer(const UINT InBufferSize, const UINT InSlotIndex, std::string InName)
{
	D3D11_BUFFER_DESC BufferDesc{};
	// now create constant buffer
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	BufferDesc.ByteWidth = InBufferSize;

	ID3D11Buffer* BufferPtr = nullptr;
	HR(GetDX11DevicePtr()->CreateBuffer(&BufferDesc, nullptr, &BufferPtr));
	std::shared_ptr<SSDX11ConstantBuffer> Result = std::make_shared<SSDX11ConstantBuffer>(BufferPtr, InSlotIndex, InBufferSize, InName);
	return Result;
}


void RHIUpdateConstantBuffer(SSDX11ConstantBuffer* InConstantBuffer, const void* InData, size_t InDataSize)
{
	check(InConstantBuffer->GetBufferSize() == InDataSize);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HR(GetDX11DeviceContextPtr()->Map(InConstantBuffer->GetDX11BufferPointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy_s(mappedResource.pData, InDataSize, InData, InDataSize);
	GetDX11DeviceContextPtr()->Unmap(InConstantBuffer->GetDX11BufferPointer(), 0);
}


void RHISetPixelShader(SSDX11PixelShader* InPixelShader)
{
	GetDX11DeviceContextPtr()->PSSetShader(InPixelShader->GetShader(), nullptr, 0);
}
void RHISetVertexShader(SSDX11VertexShader* InVertexShader)
{
	GetDX11DeviceContextPtr()->VSSetShader(InVertexShader->GetShader(), nullptr, 0);
}

void RHISetTexture(SSDX11PixelShader* InPixelShader, std::string InSlotName, SSDX11Texture2D* InTexture)
{
	int slotIndex = InPixelShader->GetTextureSlotIndex(InSlotName);
	check(slotIndex != -1);
	GetDX11DeviceContextPtr()->PSSetShaderResources(slotIndex, 1, InTexture->GetShaderResourceViewRef());
}

void RHISetTexture(SSDX11VertexShader* InVertexShader, std::string InSlotName, SSDX11Texture2D* InTexture)
{
	int slotIndex = InVertexShader->GetTextureSlotIndex(InSlotName);
	check(slotIndex != -1);
	GetDX11DeviceContextPtr()->VSSetShaderResources(slotIndex, 1, InTexture->GetShaderResourceViewRef());
}
