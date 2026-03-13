#pragma once

#include "SSDX11Texture2D.h"
#include "wrl/client.h"

using Microsoft::WRL::ComPtr;

// A GPU texture that can be bound as both SRV (t-slot) and UAV (u-slot).
// Used as intermediate / output buffers for compute shader post-processes.
// Created with D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
// it does NOT have an RTV and cannot be bound as a render target.
class SSComputeRWTexture2D : public SSDX11Texture2D
{
public:
    SSComputeRWTexture2D(UINT width, UINT height, DXGI_FORMAT format);
    virtual ~SSComputeRWTexture2D();

    void Resize(UINT width, UINT height);

    ID3D11UnorderedAccessView*        GetUnorderedAccessView()    { return mUAV.Get(); }
    ID3D11UnorderedAccessView* const* GetUnorderedAccessViewRef() { return mUAV.GetAddressOf(); }

private:
    void Create(UINT width, UINT height);

    DXGI_FORMAT                         mFormat;
    ComPtr<ID3D11UnorderedAccessView>   mUAV;
};
