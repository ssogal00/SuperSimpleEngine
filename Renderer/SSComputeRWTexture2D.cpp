
#include "SSRendererModulePCH.h"
#include "SSComputeRWTexture2D.h"
#include "SSDX11Renderer.h"

SSComputeRWTexture2D::SSComputeRWTexture2D(UINT width, UINT height, DXGI_FORMAT format)
    : mFormat(format)
{
    mTextureFormat = format;
    Create(width, height);
}

SSComputeRWTexture2D::~SSComputeRWTexture2D()
{
}

void SSComputeRWTexture2D::Create(UINT width, UINT height)
{
    ID3D11Device* device = GetDX11Device()->GetDevice();

    // Release existing resources
    mTexturePtr.Reset();
    mShaderResourceView.Reset();
    mUAV.Reset();

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width              = width;
    desc.Height             = height;
    desc.MipLevels          = 1;
    desc.ArraySize          = 1;
    desc.Format             = mFormat;
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage              = D3D11_USAGE_DEFAULT;
    desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    desc.CPUAccessFlags     = 0;
    desc.MiscFlags          = 0;

    ID3D11Texture2D* tex = nullptr;
    HR(device->CreateTexture2D(&desc, nullptr, &tex));
    mTexturePtr.Attach(tex);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format                    = mFormat;
    srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels       = 1;

    ID3D11ShaderResourceView* srv = nullptr;
    HR(device->CreateShaderResourceView(tex, &srvDesc, &srv));
    mShaderResourceView.Attach(srv);

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
    uavDesc.Format             = mFormat;
    uavDesc.ViewDimension      = D3D11_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = 0;

    ID3D11UnorderedAccessView* uav = nullptr;
    HR(device->CreateUnorderedAccessView(tex, &uavDesc, &uav));
    mUAV.Attach(uav);
}

void SSComputeRWTexture2D::Resize(UINT width, UINT height)
{
    Create(width, height);
}
