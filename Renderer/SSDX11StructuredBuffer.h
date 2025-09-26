#pragma once

#include "SSBufferBase.h"
#include "SSDX11Buffer.h"
#include "d3d11.h"

class SSDX11StructuredBuffer : public SSDX11Buffer
{
public:
	SSDX11StructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount);

	SSDX11StructuredBuffer(void* pData, unsigned int nStride, unsigned int nDataCount,
		bool bCpuWrite, bool bGpuWrite);
};
