
#include "SSDX11StructuredBuffer.h"
#include "SSDX11Renderer.h"


SSDX11StructuredBuffer::SSDX11StructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount)
{
	mBufferDescription.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	mBufferDescription.StructureByteStride = nStride;
	mBufferDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	mBufferDescription.ByteWidth = nStride * nDataCount;	
	mBufferDescription.CPUAccessFlags = 0;
	mBufferDescription.Usage = D3D11_USAGE_DEFAULT;	

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pData;

	HR(SSDX11Renderer::Get().GetDevice()->CreateBuffer(&mBufferDescription, &initData, mpBuffer.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = nDataCount;

	HR(SSDX11Renderer::Get().GetDevice()->CreateShaderResourceView(mpBuffer.Get(), &srvDesc, &mpSRV));
}

void SSDX11StructuredBuffer::UpdateBufferData(ID3D11DeviceContext* deviceContext, void* pData, unsigned int nDataSize)
{
	deviceContext->UpdateSubresource(mpBuffer.Get(), 0, nullptr, pData, 0, 0);
}

SSDX11UnorderedAccessStructuredBuffer::SSDX11UnorderedAccessStructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount)
{
	mBufferDescription.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	mBufferDescription.StructureByteStride = nStride;
	mBufferDescription.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	mBufferDescription.ByteWidth = nStride * nDataCount;	
	mBufferDescription.CPUAccessFlags = 0;
	mBufferDescription.Usage = D3D11_USAGE_DEFAULT;	
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pData;

	HR(SSDX11Renderer::Get().GetDevice()->CreateBuffer(&mBufferDescription, &initData, mpBuffer.GetAddressOf()));
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = nDataCount;

	HR(SSDX11Renderer::Get().GetDevice()->CreateUnorderedAccessView(mpBuffer.Get(), &uavDesc, &mpUAV));
}

