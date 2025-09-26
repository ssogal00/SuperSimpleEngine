#pragma once

#include "SSBufferBase.h"
#include "SSDX11Buffer.h"
#include "d3d11.h"

class SSDX11NonStructuredBuffer : public SSDX11Buffer
{
public:
	SSDX11NonStructuredBuffer(void* pData, unsigned int nDataSize, unsigned int nDataCount,
		bool bCpuWrite, bool bGpuWrite, DXGI_FORMAT eDataFormat);

	ID3D11ShaderResourceView* GetSRV() { return mpSRV; }
protected:
	ID3D11ShaderResourceView* mpSRV = nullptr;
};

// StructuredBuffer<T> in HLSL
class SSDX11StructuredBuffer : public SSDX11Buffer
{
public:
	SSDX11StructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount,
		bool bCpuWrite, bool bGpuWrite);

	ID3D11ShaderResourceView* GetSRV() { return mpSRV; }

protected:
	ID3D11ShaderResourceView* mpSRV = nullptr;
};
