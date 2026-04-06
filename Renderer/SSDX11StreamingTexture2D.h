#pragma once

#include "SSDX11Texture2D.h"
#include <atomic>

enum class EStreamingState
{
    PendingFileIO,
    LowestMipReady,
    Streaming,
    FullyLoaded,
};

class DX11RENDERER_API SSDX11StreamingTexture2D : public SSDX11Texture2D
{
public:
    SSDX11StreamingTexture2D() = default;
    virtual ~SSDX11StreamingTexture2D() = default;

    // Worker thread: file I/O only, no GPU access
    void LoadFileDataAsync(std::wstring filename, bool bSRGB);

    // Rendering thread: create GPU resource + upload lowest mip
    void CreateGPUResourceWithLowestMip();

    // Rendering thread: upload next higher-res mip
    void UploadNextMipLevel();

    // Advance blend factor each frame
    void TickStreaming(float deltaTime);

    float GetCurrentMinLOD() const;
    float GetTargetMinLOD() const;
    float GetBlendFactor() const { return mBlendFactor; }
    bool IsFullyLoaded() const { return mStreamingState == EStreamingState::FullyLoaded; }
    bool IsFileDataReady() const { return mFileDataReady.load(std::memory_order_acquire); }
    EStreamingState GetStreamingState() const { return mStreamingState; }

    // Override: return fallback SRV when GPU resource not yet created
    virtual ID3D11ShaderResourceView* GetShaderResourceView() override;
    virtual ID3D11ShaderResourceView* const* GetShaderResourceViewRef() override;

private:
    DirectX::ScratchImage mScratchImage;
    DirectX::TexMetadata mCachedMetaData{};
    bool mCachedSRGB = false;

    EStreamingState mStreamingState = EStreamingState::PendingFileIO;
    int mHighestLoadedMip = -1;
    float mBlendFactor = 0.0f;
    float mBlendDuration = 0.3f;

    std::atomic<bool> mFileDataReady{false};
};
