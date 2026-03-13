
#include "SSRendererModulePCH.h"
#include "SSGaussianBlurPostProcess.h"

#include "SSDX11Device.h"
#include "SSDX11Texture2D.h"
#include "SSShaderManager.h"
#include "SSShader.h"
#include "SSDX11Renderer.h"

SSGaussianBlurPostProcess::SSGaussianBlurPostProcess(
    UINT width, UINT height, int blurRadius, float sigma, DXGI_FORMAT format)
    : mWidth(width), mHeight(height)
    , mBlurRadius(blurRadius), mSigma(sigma)
    , mFormat(format)
{
    mHorizontalBuffer = std::make_unique<SSComputeRWTexture2D>(width, height, format);
    mVerticalBuffer   = std::make_unique<SSComputeRWTexture2D>(width, height, format);

    mBlurHCS = SSShaderManager::Get().GetComputeShader("GaussianBlurH.cs");
    mBlurVCS = SSShaderManager::Get().GetComputeShader("GaussianBlurV.cs");

    check(mBlurHCS != nullptr);
    check(mBlurVCS != nullptr);

    CreateConstantBuffer();
}

void SSGaussianBlurPostProcess::CreateConstantBuffer()
{
    ID3D11Device* device = GetDX11Device()->GetDevice();

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth           = sizeof(BlurParams);  // must be multiple of 16 — BlurParams is exactly 16 bytes
    desc.Usage               = D3D11_USAGE_DEFAULT;
    desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags      = 0;
    desc.MiscFlags           = 0;
    desc.StructureByteStride = 0;

    HR(device->CreateBuffer(&desc, nullptr, &mCB));
}

void SSGaussianBlurPostProcess::UpdateConstantBuffer(ID3D11DeviceContext* ctx)
{
    BlurParams params;
    params.width      = (int)mWidth;
    params.height     = (int)mHeight;
    params.blurRadius = mBlurRadius;
    params.sigma      = mSigma;

    ctx->UpdateSubresource(mCB, 0, nullptr, &params, 0, 0);
}

void SSGaussianBlurPostProcess::Draw(SSDX11Device* device, SSDX11Texture2D* input0)
{
    check(device  != nullptr);
    check(input0  != nullptr);
    check(mBlurHCS != nullptr);
    check(mBlurVCS != nullptr);

    ID3D11DeviceContext* ctx = device->GetDeviceContext();

    UpdateConstantBuffer(ctx);

    const UINT groupsX = (mWidth  + 7) / 8;
    const UINT groupsY = (mHeight + 7) / 8;

    // --------------------------------------------------------
    // Pass 1 — Horizontal blur
    //   Input  : input0 SRV          (t0)
    //   Output : mHorizontalBuffer   (u0)
    // --------------------------------------------------------
    ctx->CSSetShader(mBlurHCS->GetShader(), nullptr, 0);
    ctx->CSSetConstantBuffers(0, 1, &mCB);

    ID3D11ShaderResourceView* srvH = input0->GetShaderResourceView();
    ctx->CSSetShaderResources(0, 1, &srvH);

    ID3D11UnorderedAccessView* uavH = mHorizontalBuffer->GetUnorderedAccessView();
    ctx->CSSetUnorderedAccessViews(0, 1, &uavH, nullptr);

    ctx->Dispatch(groupsX, groupsY, 1);

    // Unbind to avoid hazards before Pass 2
    ID3D11ShaderResourceView*  nullSRV = nullptr;
    ID3D11UnorderedAccessView* nullUAV = nullptr;
    ctx->CSSetShaderResources(0, 1, &nullSRV);
    ctx->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);

    // --------------------------------------------------------
    // Pass 2 — Vertical blur
    //   Input  : mHorizontalBuffer SRV  (t0)
    //   Output : mVerticalBuffer        (u0)
    // --------------------------------------------------------
    ctx->CSSetShader(mBlurVCS->GetShader(), nullptr, 0);
    // constant buffer is already bound to slot 0

    ID3D11ShaderResourceView* srvV = mHorizontalBuffer->GetShaderResourceView();
    ctx->CSSetShaderResources(0, 1, &srvV);

    ID3D11UnorderedAccessView* uavV = mVerticalBuffer->GetUnorderedAccessView();
    ctx->CSSetUnorderedAccessViews(0, 1, &uavV, nullptr);

    ctx->Dispatch(groupsX, groupsY, 1);

    // Unbind compute state to leave the pipeline clean
    ctx->CSSetShader(nullptr, nullptr, 0);
    ctx->CSSetShaderResources(0, 1, &nullSRV);
    ctx->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
    ID3D11Buffer* nullCB = nullptr;
    ctx->CSSetConstantBuffers(0, 1, &nullCB);
}

void SSGaussianBlurPostProcess::OnResize(UINT width, UINT height)
{
    mWidth  = width;
    mHeight = height;

    mHorizontalBuffer->Resize(width, height);
    mVerticalBuffer->Resize(width, height);
}
