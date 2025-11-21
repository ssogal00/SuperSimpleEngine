#pragma once

#include "SSBufferBase.h"
#include "d3d11.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class SSDX11Buffer : public SSBufferBase
{
public:  
    virtual void* const* GetBufferPointerRef() override;
    ID3D11Buffer* const* GetDX11BufferPointerRef() { return mpBuffer.GetAddressOf(); }
    virtual BYTE* GetBufferDataPtr() override { return nullptr; }
    ID3D11Buffer* GetDX11BufferPointer() { return mpBuffer.Get(); }
    virtual void* GetBufferPointer() override;
    virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext){}   
	virtual bool IsDirty() const { return bDirty; }
protected:

    D3D11_BUFFER_DESC mBufferDescription{};
    ComPtr<ID3D11Buffer> mpBuffer = nullptr;
    bool bDirty = false;
};


