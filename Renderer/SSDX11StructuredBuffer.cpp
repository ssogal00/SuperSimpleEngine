
#include "SSDX11StructuredBuffer.h"
#include "SSDX11Renderer.h"



SSDX11StructuredBuffer::SSDX11StructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount,
	bool bCpuWrite, bool bGpuWrite)
{
	mBufferDescription.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	mBufferDescription.StructureByteStride = nStride;
	mBufferDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	mBufferDescription.ByteWidth = nStride * nDataCount;	

	if (bCpuWrite)
	{
		mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	}
	else
	{
		mBufferDescription.CPUAccessFlags = 0;
		mBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	}

	if (bGpuWrite)
	{
		mBufferDescription.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

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