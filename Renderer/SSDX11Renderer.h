#pragma once

#include <memory>
#include "SSDX11Device.h"
#include "SSRendererModulePCH.h"
#include "SSRenderer.h"

class DX11RENDERER_API SSDX11Renderer : public SSRenderer
{
public:
	SSDX11Renderer();	
	virtual ~SSDX11Renderer(){}
	virtual void Initialize(HWND windowHandle) override;
	virtual void Shutdown() override;
	virtual void OnWindowResize(int newWidth, int newHeight) override;
	virtual void DrawScene() override;
	virtual void UpdateRenderingObjects() override;

	void SetWindowWidth(int inWidth);
	void SetWindowHeight(int inHeight);
	
	static SSDX11Renderer& Get();
	static SSDX11Renderer* GetPtr();	

	void AppendRenderCommand(class SSRenderCmdBase* cmd);
	void FlushRenderCommands();

	DXGI_FORMAT SwapChainFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	static SSDX11Device* GetDX11Device() { return mDX11Device; }

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetImmediateDeviceContext();
	IDXGISwapChain* GetSwapChain();

	void SetGBufferDump(bool bDump) { bGbufferDump = bDump; }
	void ToggleGBufferDumpMode() { bGbufferDump = !bGbufferDump; }

protected:
	void DrawCubeScene();
	void DrawObject();
	void DrawSkybox();
	void DrawDummyScene();	

	void CreateSkyBoxDrawCommands();
	
	std::shared_ptr<class SSViewport> mViewport = nullptr;

	std::vector<class SSRenderCmdBase*> mRenderCommandList;

	static SSDX11Renderer* mRendererInstance;

	void TestCompileShader();
	void TestCreateResources();

	int mWindowWidth = 0;
	int mWindowHeight = 0;

private:		
	bool bInitialized = false;
	bool bGbufferDump = false;

private:
	static SSDX11Device* mDX11Device;	

	ID3D11SamplerState* mDefaultSamplerState = nullptr;	

	std::shared_ptr<class SSRenderTargetCube> mRenderTargetCube;
	std::vector<std::shared_ptr<class SSSPhere>> mSphereList;

	std::shared_ptr<class SSCubeMapRenderingSphere> mCubemapSphere;

	std::shared_ptr<class SSScreenBlit> mScreenBlit;
	// 	

	std::shared_ptr<class SSDX11VertexShader> mScreenBlitVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mScreenBlitPixelShader;

	std::shared_ptr<class SSDX11VertexShader> mDeferredVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mDeferredPixelShader;

	std::shared_ptr<class SSDX11VertexShader> mDeferredLightVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mDeferredLightPixelShader;

	std::shared_ptr<class SSTextureCube> mTestCubeTexture;

	std::shared_ptr<class SSDX11VertexShader> mCubemapVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mCubemapPixelShader;

	std::shared_ptr<class SSDX11VertexShader> mEquirectToCubemapVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mEquirectToCubemapPixelShader;

	std::shared_ptr<class SSDX11VertexShader> mCubemapConvolutionVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mCubemapConvolutionPixelShader;

	std::shared_ptr<class SSDX11VertexShader> mPrefilterVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mPrefilterPixelShader;

	std::shared_ptr<class SSDX11VertexShader> m2DLUTVertexShader;
	std::shared_ptr<class SSDX11PixelShader> m2DLUTPixelShader;

	std::shared_ptr<class SSDX11VertexShader> mTBNDebugVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mTBNDebugPixelShader;

	std::shared_ptr<class SSPBRSphere> mRustedIron;
	std::shared_ptr<class SSPBRSphere> mTile;
	std::shared_ptr<class SSPBRSphere> mMetalGrid;


	// @ cubemap rendertargets
	std::shared_ptr<class SSCubemapRenderTarget> mEquirectToCubemapRenderTarget;
	std::shared_ptr<class SSCubemapRenderTarget> mConvolutionRenderTarget;
	std::shared_ptr<class SSPrefilterCubemapRenderTarget> mPrefilterRenderTarget;

	std::shared_ptr<class SSTextureCube> mEnvCubemap;
	std::shared_ptr<class SSTextureCube> mEnvCubemapConvolution;
	std::shared_ptr<class SSTextureCube> mEnvCubemapPrefilter;
	std::shared_ptr<class SSDX11Texture2D> m2DLUTTexture;
	// @
	
	std::shared_ptr<class SSDX11Texture2D> mHDREnvmap;

	std::shared_ptr<class SSDX11GBuffer> mGBuffer;

	std::shared_ptr<class SSDX11RenderTarget> m2DLUTRenderTarget;
	//

	std::shared_ptr<class SSObjMesh> mSponzaMesh;
	std::shared_ptr<class SSObjMesh> mObjMeshSphere;

	std::shared_ptr<class SSDX11PostProcess> mFXAAPostProcess;
	std::shared_ptr<class SSGaussianBlurPostProcess> mGaussianBlurPostProcess;
	std::shared_ptr<class SSGBufferDumpPostProcess> mGBufferDumpProcess;
	std::shared_ptr<class SSDX11PostProcess> mDeferredLightPostProcess;

	

private:
	bool TryLoadEnvCubemap(std::wstring filepath);
	bool TryLoadEnvCubemapConvolution(std::wstring filepath);
	bool TryLoadEnvCubemapPrefilter(std::wstring filepath);
	bool TryLoad2DLUTTexture();

	void CreateEnvCubemap();
	void CreateEnvCubemapConvolution();
	void CreateEnvCubemapPrefilter();
	void Create2DLUTTexture();
	void Resize(int width, int height);
};

inline SSDX11Device* GetDX11Device() { return SSDX11Renderer::Get().GetDX11Device(); }