
#include "SSRendererModulePCH.h"
#include "SSTextureStreamingManager.h"
#include "SSDX11StreamingTexture2D.h"
#include "SSDX11Renderer.h"

void SSTextureStreamingManager::Initialize()
{
    CreateFallbackTexture();
}

void SSTextureStreamingManager::Shutdown()
{
    mPendingList.clear();
    mStreamingQueue.clear();
    mFallbackTex.Reset();
    mFallbackSRV.Reset();
}

void SSTextureStreamingManager::CreateFallbackTexture()
{
    auto* device = SSDX11Renderer::Get().GetDevice();

    constexpr UINT texSize = 16;
    constexpr UINT checkerSize = 2; // 2x2 pixels per checker cell

    // Dark gray checkerboard: alternating between two gray tones
    constexpr UINT colorA = 0xFF4A4A4A; // ABGR: darker gray (R=74, G=74, B=74)
    constexpr UINT colorB = 0xFF5E5E5E; // ABGR: lighter gray (R=94, G=94, B=94)

    UINT pixels[texSize * texSize];
    for (UINT y = 0; y < texSize; ++y)
    {
        for (UINT x = 0; x < texSize; ++x)
        {
            bool isLight = ((x / checkerSize) + (y / checkerSize)) % 2 == 0;
            pixels[y * texSize + x] = isLight ? colorA : colorB;
        }
    }

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = texSize;
    desc.Height = texSize;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = pixels;
    initData.SysMemPitch = texSize * sizeof(UINT);
    initData.SysMemSlicePitch = 0;

    HR(device->CreateTexture2D(&desc, &initData, &mFallbackTex));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    HR(device->CreateShaderResourceView(mFallbackTex.Get(), &srvDesc, &mFallbackSRV));
}

std::shared_ptr<SSDX11StreamingTexture2D> SSTextureStreamingManager::RequestStreamingTexture(
    const std::wstring& path, bool bSRGB)
{
    auto streamingTex = std::make_shared<SSDX11StreamingTexture2D>();

    mPendingList.push_back(streamingTex);

    // Use raw pointer in lambda - object lifetime is guaranteed by mPendingList
    SSDX11StreamingTexture2D* rawPtr = streamingTex.get();
    std::wstring pathCapture = path;
    bool srgbCapture = bSRGB;

    mThreadPool.EnqueueJob([rawPtr, pathCapture, srgbCapture]()
    {
        rawPtr->LoadFileDataAsync(pathCapture, srgbCapture);
    });

    return streamingTex;
}

void SSTextureStreamingManager::Tick(float deltaTime)
{
    // Phase 1: Check pending textures for file I/O completion
    for (auto it = mPendingList.begin(); it != mPendingList.end(); )
    {
        if ((*it)->IsFileDataReady())
        {
            (*it)->CreateGPUResourceWithLowestMip();
            mStreamingQueue.push_back(*it);
            it = mPendingList.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Phase 2: Progressive mip uploads
    int uploadsThisFrame = 0;
    for (auto it = mStreamingQueue.begin(); it != mStreamingQueue.end(); )
    {
        auto& tex = *it;
        tex->TickStreaming(deltaTime);

        // When blend is complete and more mips remain, upload next mip
        if (tex->GetBlendFactor() >= 1.0f && !tex->IsFullyLoaded()
            && uploadsThisFrame < mMaxMipUploadsPerFrame)
        {
            tex->UploadNextMipLevel();
            uploadsThisFrame++;
        }

        if (tex->IsFullyLoaded())
        {
            it = mStreamingQueue.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
