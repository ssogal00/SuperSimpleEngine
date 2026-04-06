#pragma once

#include "SSRendererModulePCH.h"
#include "Singleton.h"
#include "SSManagerBase.h"
#include "SSThreadPool.h"
#include <memory>
#include <vector>
#include <string>
#include <wrl/client.h>

class SSDX11StreamingTexture2D;
class SSDX11Texture2D;

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSTextureStreamingManager : public Singleton<SSTextureStreamingManager>, public SSManagerBase
{
public:
    virtual void Initialize() override;
    virtual void Shutdown() override;

    // Request a streaming texture. Returns immediately with a texture that
    // initially shows a fallback, then progressively loads mip levels.
    std::shared_ptr<SSDX11StreamingTexture2D> RequestStreamingTexture(
        const std::wstring& path, bool bSRGB);

    // Called on the rendering thread each frame
    void Tick(float deltaTime);

    ID3D11ShaderResourceView* GetFallbackSRV() { return mFallbackSRV.Get(); }
    ID3D11ShaderResourceView* const* GetFallbackSRVRef() { return mFallbackSRV.GetAddressOf(); }

private:
    // Textures waiting for file I/O to complete
    std::vector<std::shared_ptr<SSDX11StreamingTexture2D>> mPendingList;

    // Textures with progressive mip upload in progress
    std::vector<std::shared_ptr<SSDX11StreamingTexture2D>> mStreamingQueue;

    int mMaxMipUploadsPerFrame = 2;

    // 1x1 white fallback texture
    ComPtr<ID3D11Texture2D> mFallbackTex;
    ComPtr<ID3D11ShaderResourceView> mFallbackSRV;

    // Thread pool for async file I/O
    SSThreadPool mThreadPool;

    void CreateFallbackTexture();
};
