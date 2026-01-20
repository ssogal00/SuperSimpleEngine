#pragma once

#include "SSBufferBase.h"
#include "SSDX11Buffer.h"
#include "d3d11.h"

using Microsoft::WRL::ComPtr;

// StructuredBuffer<T> in HLSL
// gpu read only
class SSDX11StructuredBuffer : public SSDX11Buffer
{
public:
	SSDX11StructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount);
	ID3D11ShaderResourceView* GetSRV() { return mpSRV; }

	void UpdateBufferData(ID3D11DeviceContext* deviceContext, void* InDataPtr, unsigned int InDataLength);

protected:
	ID3D11ShaderResourceView* mpSRV = nullptr;
	BYTE* mBufferData = nullptr;
};

// RWStructuredBuffer<T> in HLSL
// gpu read write 
class SSDX11UnorderedAccessStructuredBuffer : public SSDX11Buffer
{
public:
	SSDX11UnorderedAccessStructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount);

	ID3D11UnorderedAccessView* GetUAV() { return mpUAV; }

protected:
	ID3D11UnorderedAccessView* mpUAV = nullptr;
};