
#include "SSRHI.h"
#include "SSDX11IndexBuffer.h"
#include "SSDX11Renderer.h"

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