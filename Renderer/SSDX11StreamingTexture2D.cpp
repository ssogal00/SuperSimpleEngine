
#include "SSRendererModulePCH.h"
#include "SSDX11StreamingTexture2D.h"
#include "SSTextureStreamingManager.h"
#include "SSDX11Renderer.h"
#include "DirectXTex.h"

void SSDX11StreamingTexture2D::LoadFileDataAsync(std::wstring filename, bool bSRGB)
{
    mCachedSRGB = bSRGB;
    mStreamingState = EStreamingState::PendingFileIO;

    // CPU-only file read (called on worker thread)
    HRESULT hr = DirectX::LoadFromDDSFile(
        filename.c_str(),
        DirectX::DDS_FLAGS_NONE,
        &mCachedMetaData,
        mScratchImage
    );

    if (SUCCEEDED(hr))
    {
        mFileDataReady.store(true, std::memory_order_release);
    }
}

void SSDX11StreamingTexture2D::CreateGPUResourceWithLowestMip()
{
    mWidth = static_cast<UINT>(mCachedMetaData.width);
    mHeight = static_cast<UINT>(mCachedMetaData.height);
    mMipLevels = static_cast<UINT>(mCachedMetaData.mipLevels);

    D3D11_TEXTURE2D_DESC description;
    description.Width = mWidth;
    description.Height = mHeight;
    description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    description.MiscFlags = 0;
    description.Usage = D3D11_USAGE_DEFAULT;
    description.SampleDesc.Count = 1;
    description.SampleDesc.Quality = 0;
    description.MipLevels = static_cast<UINT>(mCachedMetaData.mipLevels);
    description.ArraySize = static_cast<UINT>(mCachedMetaData.arraySize);
    description.CPUAccessFlags = 0;

    this->bSRGB = mCachedSRGB;

    if (bSRGB)
    {
        mTextureFormat = description.Format = DirectX::MakeSRGB(mCachedMetaData.format);
    }
    else
    {
        mTextureFormat = description.Format = mCachedMetaData.format;
    }

    HR(SSDX11Renderer::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
    ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    resourceViewDesc.Format = description.Format;
    resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDesc.Texture2D.MostDetailedMip = 0;
    resourceViewDesc.Texture2D.MipLevels = static_cast<UINT>(mCachedMetaData.mipLevels);

    HR(SSDX11Renderer::Get().GetDevice()->CreateShaderResourceView(mTexturePtr.Get(), &resourceViewDesc, &mShaderResourceView));

    // Upload only the lowest resolution mip
    int lowestMip = static_cast<int>(mCachedMetaData.mipLevels) - 1;
    auto* pLodImage = mScratchImage.GetImage(lowestMip, 0, 0);

    if (pLodImage)
    {
        auto dstSubresource = D3D11CalcSubresource(lowestMip, 0, static_cast<UINT>(mCachedMetaData.mipLevels));
        SSDX11Renderer::Get().GetImmediateDeviceContext()->UpdateSubresource(
            mTexturePtr.Get(),
            dstSubresource,
            nullptr,
            pLodImage->pixels,
            static_cast<UINT>(pLodImage->rowPitch),
            0
        );
    }

    mHighestLoadedMip = lowestMip;
    mBlendFactor = 1.0f;
    mStreamingState = EStreamingState::LowestMipReady;
}

void SSDX11StreamingTexture2D::UploadNextMipLevel()
{
    int nextMip = mHighestLoadedMip - 1;
    if (nextMip < 0) return;

    auto* pLodImage = mScratchImage.GetImage(nextMip, 0, 0);

    if (pLodImage)
    {
        auto dstSubresource = D3D11CalcSubresource(nextMip, 0, static_cast<UINT>(mCachedMetaData.mipLevels));
        SSDX11Renderer::Get().GetImmediateDeviceContext()->UpdateSubresource(
            mTexturePtr.Get(),
            dstSubresource,
            nullptr,
            pLodImage->pixels,
            static_cast<UINT>(pLodImage->rowPitch),
            0
        );
    }

    mHighestLoadedMip = nextMip;
    mBlendFactor = 0.0f;
    mStreamingState = EStreamingState::Streaming;

    if (mHighestLoadedMip == 0)
    {
        mStreamingState = EStreamingState::FullyLoaded;
        mBlendFactor = 1.0f;
        mScratchImage.Release();
    }
}

void SSDX11StreamingTexture2D::TickStreaming(float deltaTime)
{
    if (mStreamingState == EStreamingState::FullyLoaded ||
        mStreamingState == EStreamingState::PendingFileIO)
    {
        return;
    }

    if (mBlendFactor < 1.0f)
    {
        mBlendFactor += deltaTime / mBlendDuration;
        if (mBlendFactor > 1.0f)
        {
            mBlendFactor = 1.0f;
        }
    }
}

float SSDX11StreamingTexture2D::GetCurrentMinLOD() const
{
    if (mStreamingState == EStreamingState::FullyLoaded)
    {
        return 0.0f;
    }
    return static_cast<float>(mHighestLoadedMip);
}

float SSDX11StreamingTexture2D::GetTargetMinLOD() const
{
    if (mStreamingState == EStreamingState::FullyLoaded)
    {
        return 0.0f;
    }
    int target = mHighestLoadedMip - 1;
    return static_cast<float>(target >= 0 ? target : 0);
}

ID3D11ShaderResourceView* SSDX11StreamingTexture2D::GetShaderResourceView()
{
    if (mShaderResourceView)
    {
        return mShaderResourceView.Get();
    }
    return SSTextureStreamingManager::Get().GetFallbackSRV();
}

ID3D11ShaderResourceView* const* SSDX11StreamingTexture2D::GetShaderResourceViewRef()
{
    if (mShaderResourceView)
    {
        return mShaderResourceView.GetAddressOf();
    }
    return SSTextureStreamingManager::Get().GetFallbackSRVRef();
}
