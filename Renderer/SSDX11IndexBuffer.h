
#pragma once

#include <vector>
#include "SSIndexBuffer.h"

class SSDX11IndexBuffer : public SSIndexBuffer
{
public:

	SSDX11IndexBuffer(ID3D11Buffer* ptrBuffer, unsigned int nIndexCount);
    //SSDX11IndexBuffer(){}

	virtual ~SSDX11IndexBuffer();
	virtual void* GetBufferPointer() override;
	virtual void* const* GetBufferPointerRef() override;
	virtual BYTE* GetBufferDataPtr() override { return nullptr; }

protected:
	D3D11_BUFFER_DESC mBufferDescription;
	ComPtr<ID3D11Buffer> mpBuffer = nullptr;
};