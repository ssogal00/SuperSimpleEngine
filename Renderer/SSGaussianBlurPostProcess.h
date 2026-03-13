#pragma once

#include "SSComputeRWTexture2D.h"
#include <memory>

class SSDX11Device;
class SSDX11Texture2D;
class SSDX11ComputeShader;

// Separable Gaussian blur implemented with two compute shader passes:
//   Pass 1 (GaussianBlurH.cs): horizontal blur  → mHorizontalBuffer
//   Pass 2 (GaussianBlurV.cs): vertical   blur  → mVerticalBuffer
//
// Usage:
//   mGaussianBlur->Draw(device, inputTexture);
//   SSDX11Texture2D* result = mGaussianBlur->GetOutput();
class SSGaussianBlurPostProcess
{
public:
    SSGaussianBlurPostProcess(UINT width, UINT height,
                              int   blurRadius = 7,
                              float sigma      = 2.5f,
                              DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);

    // Run both blur passes.  input0 must have a valid SRV.
    void Draw(SSDX11Device* device, SSDX11Texture2D* input0);

    // Returns the fully blurred result (SRV ready for the next pass / FXAA etc.)
    SSDX11Texture2D* GetOutput() { return mVerticalBuffer.get(); }

    void OnResize(UINT width, UINT height);

    // Tune at runtime
    void SetBlurRadius(int radius) { mBlurRadius = radius; }
    void SetSigma(float sigma)     { mSigma      = sigma;  }

private:
    void CreateConstantBuffer();
    void UpdateConstantBuffer(ID3D11DeviceContext* ctx);

    UINT        mWidth;
    UINT        mHeight;
    int         mBlurRadius;
    float       mSigma;
    DXGI_FORMAT mFormat;

    // Ping-pong UAV textures
    std::unique_ptr<SSComputeRWTexture2D> mHorizontalBuffer; // output of H pass
    std::unique_ptr<SSComputeRWTexture2D> mVerticalBuffer;   // output of V pass

    // Compute shaders (shared_ptr owned by SSShaderManager)
    std::shared_ptr<SSDX11ComputeShader> mBlurHCS;
    std::shared_ptr<SSDX11ComputeShader> mBlurVCS;

    // Simple constant buffer (b0) — layout must match CBBlurParams in both .cs files
    struct BlurParams
    {
        int   width;
        int   height;
        int   blurRadius;
        float sigma;
    };
    ID3D11Buffer* mCB = nullptr;
};
