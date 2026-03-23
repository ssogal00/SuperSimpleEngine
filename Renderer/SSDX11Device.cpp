#include "SSDX11Device.h"
#include "SSDX11Device.h"
#include <iostream>
#include "SSDX11RenderTarget.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSShader.h"
#include "SSCameraManager.h"
#include <filesystem>

bool SSDX11Device::CreateDevice()
{
	IDXGIFactory* pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (pFactory)
	{
		IDXGIAdapter* pAdapter = nullptr;
		HRESULT AdapterResult = S_FALSE;
		UINT i = 0;
		do
		{
			AdapterResult = pFactory->EnumAdapters(i, &pAdapter);
			if (AdapterResult == S_OK)
			{
				DXGI_ADAPTER_DESC desc;
				pAdapter->GetDesc(&desc);

				mAdapterInfos.push_back(SSAdapterInfo{ desc, pAdapter });
			}
			i++;
		} while (AdapterResult == S_OK);
	}

	D3D_FEATURE_LEVEL featureLevelArray[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	UINT length = sizeof(featureLevelArray) / sizeof(D3D_FEATURE_LEVEL);

	D3D_FEATURE_LEVEL outFeatureLevel;

	HRESULT hr = D3D11CreateDevice(mAdapterInfos[0].AdapterPointer,
		D3D_DRIVER_TYPE_UNKNOWN,
		0,
		0,
		featureLevelArray,
		length,
		D3D11_SDK_VERSION, mDevice.GetAddressOf(), &outFeatureLevel, mDeviceContext.ReleaseAndGetAddressOf());

	if (hr == 0)
	{
		std::cout << "Device created" << std::endl;
	}

	return true;
}


ID3D11Device* SSDX11Device::GetDevice() const
{
	return mDevice.Get();
}

ID3D11DeviceContext* SSDX11Device::GetDeviceContext() const
{
	return mDeviceContext.Get();
}

IDXGISwapChain* SSDX11Device::GetSwapChain() const
{
	return mSwapChain.Get();
}


bool SSDX11Device::InitializeDevice(HWND windowHandle)
{
	bool bDeviceCreated = CreateDevice();
	bool bSwapChainCreated = CreateSwapChain(windowHandle);

	return bDeviceCreated && bSwapChainCreated;
}


std::shared_ptr<SSDX11VertexBuffer> SSDX11Device::CreateVertexBuffer(unsigned int stride, unsigned int count, void* ptrData)
{
	D3D11_BUFFER_DESC bufferDesc{};

	bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = static_cast<UINT>(stride * count);

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = ptrData;
	vertexSubresourceData.SysMemPitch = 0;
	vertexSubresourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* PtrBuffer = nullptr;

	HR(mDevice->CreateBuffer(&bufferDesc, &vertexSubresourceData, &PtrBuffer));

	std::shared_ptr<SSDX11VertexBuffer> Result = make_shared<SSDX11VertexBuffer>(PtrBuffer, stride, count);

	return Result;
}

std::shared_ptr<SSDX11IndexBuffer> SSDX11Device::CreateIndexBuffer(std::vector<unsigned int>& inData)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * inData.size());

	D3D11_SUBRESOURCE_DATA indexSubresourceData;
	indexSubresourceData.pSysMem = &inData[0];
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* ptrBuffer = nullptr;

	HR(mDevice->CreateBuffer(&bufferDesc, &indexSubresourceData, &ptrBuffer));

	std::shared_ptr<SSDX11IndexBuffer> Result = make_shared<SSDX11IndexBuffer>(ptrBuffer,static_cast<unsigned int>(inData.size()));

	return Result;
}


std::shared_ptr<SSDX11IndexBuffer> SSDX11Device::CreateIndexBuffer(std::vector<unsigned short>& inData)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(unsigned short);
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * inData.size());

	D3D11_SUBRESOURCE_DATA indexSubresourceData;
	indexSubresourceData.pSysMem = &inData[0];
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* ptrBuffer = nullptr;

	HR(mDevice->CreateBuffer(&bufferDesc, &indexSubresourceData, &ptrBuffer));

	std::shared_ptr<SSDX11IndexBuffer> Result = make_shared<SSDX11IndexBuffer>(ptrBuffer, static_cast<unsigned int>(inData.size()));

	return Result;
}

SSDX11RenderTargetTexture2D* SSDX11Device::CreateRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat, bool bGeneratedMips, UINT maxMipCount)
{
	auto Tuple = InternalCreateRenderTargetTexture2D(width, height, eFormat, maxMipCount);

	SSDX11RenderTargetTexture2D* Result = new SSDX11RenderTargetTexture2D(
		std::get<0>(Tuple),
		std::get<1>(Tuple),
		std::get<2>(Tuple),
		width, 
		height, 
		eFormat, 
		bGeneratedMips, 
		maxMipCount);
	
	return Result;
}

void SSDX11Device::ResizeRenderTargetTexture2D(SSDX11RenderTargetTexture2D* InRT2D, const UINT InWidth, const UINT InHeight)
{
	InRT2D->Destroy();
	
	auto Result = InternalCreateRenderTargetTexture2D(InWidth, InHeight, InRT2D->GetTextureFormat(), InRT2D->GetMipLevels());
	
	InRT2D->mTexturePtr = std::get<0>(Result);
	InRT2D->mShaderResourceView = std::get<1>(Result);
	std::vector<ID3D11RenderTargetView*> RenderTargetViewArray = std::get<2>(Result);
	
	for (UINT i = 0; i < InRT2D->GetMipLevels(); i++)
	{
		InRT2D->mRenderTargetView[i] = RenderTargetViewArray[i];
		InRT2D->mRenderTargetView[i]->AddRef();
	}

	InRT2D->mWidth = InWidth;
	InRT2D->mHeight = InHeight;
}

std::tuple<ID3D11Texture2D*, ID3D11ShaderResourceView*, std::vector<ID3D11RenderTargetView*>>
SSDX11Device::InternalCreateRenderTargetTexture2D(const UINT InWidth, const UINT InHeight, DXGI_FORMAT eFormat, UINT maxMipCount)
{	
	const UINT Mips = maxMipCount;

	check(Mips > 0);

	const DXGI_FORMAT Format = eFormat;

	D3D11_TEXTURE2D_DESC textureDesc{ 0 };

	ID3D11Texture2D* TexturePtr = nullptr;	
	ID3D11ShaderResourceView* ShaderResourceView = nullptr;

	textureDesc.Width = InWidth;
	textureDesc.Height = InHeight;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	if (Mips > 1)
	{
		check(InWidth == InHeight);

		bool bPowerOfTwo = !(InWidth == 0) && !(InWidth & (InWidth - 1));
		check(bPowerOfTwo);

		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		textureDesc.MipLevels = Mips;
	}
	else
	{
		textureDesc.MiscFlags = 0;
		textureDesc.MipLevels = 1;
	}

	std::vector<ID3D11RenderTargetView*> RenderTargetView;
	RenderTargetView.resize(Mips);

	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.SampleDesc.Count = 1;

	textureDesc.ArraySize = 1;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = Format;

	HR(mDevice->CreateTexture2D(&textureDesc, nullptr, &TexturePtr));

	for (UINT i = 0; i < Mips; ++i)
	{
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
		ZeroMemory(&renderTargetDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		renderTargetDesc.Format = textureDesc.Format;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D.MipSlice = i;

		HR(mDevice->CreateRenderTargetView(TexturePtr, &renderTargetDesc, &RenderTargetView[i]));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = Mips;

	HR(mDevice->CreateShaderResourceView(TexturePtr, &shaderResourceViewDesc, &ShaderResourceView));

	return std::tuple<ID3D11Texture2D*, ID3D11ShaderResourceView*, std::vector<ID3D11RenderTargetView*>>{TexturePtr, ShaderResourceView, RenderTargetView};
}


void SSDX11Device::ClearDefaultRenderTargetView(float color[4])
{	
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);	
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void SSDX11Device::SetDefaultRenderTargetAsCurrent()
{	
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetViewports(1, &mScreenViewport);
}

bool SSDX11Device::IsBoundPixelShaderEqual(ID3D11PixelShader* InPS)
{
	if (mDeviceStateCache.mPixelShader == InPS)
	{
		return true;
	}

	return false;
}

bool SSDX11Device::UpdateBoundPixelShaderIfDifferent(ID3D11PixelShader* InPS)
{
	if (mDeviceStateCache.mPixelShader != InPS)
	{
		mDeviceContext->PSSetShader(InPS, nullptr, 0);
		mDeviceStateCache.mPixelShader = InPS;
		return true;
	}
	return false;
}

bool SSDX11Device::UpdateBoundVertexShaderIfDifferent(ID3D11VertexShader* InVS)
{
	if (mDeviceStateCache.mVertexShader != InVS)
	{
		mDeviceContext->VSSetShader(InVS, nullptr, 0);
		mDeviceStateCache.mVertexShader = InVS;
		return true;
	}
	return false;
}

bool SSDX11Device::UpdateBoundComputeShaderIfDifferent(ID3D11ComputeShader* InCS)
{
	if (mDeviceStateCache.mComputeShader != InCS)
	{
		mDeviceContext->CSSetShader(InCS, nullptr, 0);
		mDeviceStateCache.mComputeShader = InCS;
		return true;
	}
	return false;
}

bool SSDX11Device::SetPSBoundConstantBufferIfDifferent(UINT InSlotIndex, ID3D11Buffer* InBuffer)
{
	// not exist
	if (mDeviceStateCache.mBoundPSConstantBuffers.find(InSlotIndex) == mDeviceStateCache.mBoundPSConstantBuffers.end())
	{
		mDeviceContext->PSSetConstantBuffers(InSlotIndex, 1, &InBuffer);
		mDeviceStateCache.mBoundPSConstantBuffers[InSlotIndex] = InBuffer;
		return true;
	}
	// exist but different
	if (mDeviceStateCache.mBoundPSConstantBuffers[InSlotIndex] != InBuffer)
	{
		mDeviceContext->PSSetConstantBuffers(InSlotIndex, 1, &InBuffer);
		mDeviceStateCache.mBoundPSConstantBuffers[InSlotIndex] = InBuffer;
		return true;
	}
	return false;
}

bool SSDX11Device::SetVSBoundConstantBufferIfDifferent(UINT InSlotIndex, ID3D11Buffer* InBuffer)
{
	// not exist
	if(mDeviceStateCache.mBoundVSConstantBuffers.find(InSlotIndex) == mDeviceStateCache.mBoundVSConstantBuffers.end())
	{
		mDeviceContext->VSSetConstantBuffers(InSlotIndex, 1, &InBuffer);
		mDeviceStateCache.mBoundVSConstantBuffers[InSlotIndex] = InBuffer;
		return true;
	}

	// exist but different
	if (mDeviceStateCache.mBoundVSConstantBuffers[InSlotIndex] != InBuffer)
	{
		mDeviceContext->VSSetConstantBuffers(InSlotIndex, 1, &InBuffer);
		mDeviceStateCache.mBoundVSConstantBuffers[InSlotIndex] = InBuffer;
		return true;
	}

	return false;
}

bool SSDX11Device::SetPSBoundTextureIfDifferent(UINT InSlotIndex, ID3D11ShaderResourceView* InTextureView)
{
	if(mDeviceStateCache.mBoundPSTextures.find(InSlotIndex) == mDeviceStateCache.mBoundPSTextures.end())
	{
		mDeviceContext->PSSetShaderResources(InSlotIndex, 1, &InTextureView);
		mDeviceStateCache.mBoundPSTextures[InSlotIndex] = InTextureView;
		return true;
	}

	if(mDeviceStateCache.mBoundPSTextures[InSlotIndex] != InTextureView)
	{
		mDeviceContext->PSSetShaderResources(InSlotIndex, 1, &InTextureView);
		mDeviceStateCache.mBoundPSTextures[InSlotIndex] = InTextureView;
		return true;
	}

	return false;
}

bool SSDX11Device::SetVSBoundTextureIfDifferent(UINT InSlotIndex, ID3D11ShaderResourceView* InTextureView)
{
	if(mDeviceStateCache.mBoundVSTextures.find(InSlotIndex) == mDeviceStateCache.mBoundVSTextures.end())
	{
		mDeviceContext->VSSetShaderResources(InSlotIndex, 1, &InTextureView);
		mDeviceStateCache.mBoundVSTextures[InSlotIndex] = InTextureView;
		return true;
	}

	if(mDeviceStateCache.mBoundVSTextures[InSlotIndex] != InTextureView)
	{
		mDeviceContext->VSSetShaderResources(InSlotIndex, 1, &InTextureView);
		mDeviceStateCache.mBoundVSTextures[InSlotIndex] = InTextureView;
		return true;
	}

	return false;
}

void SSDX11Device::Present()
{
	HRESULT presentResult = mSwapChain->Present(0, 0);

	if (presentResult == DXGI_ERROR_DEVICE_HUNG ||
		presentResult == DXGI_ERROR_DEVICE_REMOVED ||
		presentResult == DXGI_ERROR_DEVICE_RESET)
	{
		HRESULT removedReason = mDevice->GetDeviceRemovedReason();
		wchar_t outString[100];
		size_t size = 100;
		swprintf_s(outString, size, L"Device removed! DXGI_ERROR code: 0x%X\n", removedReason);
		OutputDebugStringW(outString);
	}
}

void SSDX11Device::ResizeRenderTarget(int inWidth, int inHeight)
{
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.MaxDepth = 1;
	mScreenViewport.MinDepth = 0;
	mScreenViewport.Width = static_cast<float>(inWidth);
	mScreenViewport.Height = static_cast<float>(inHeight);
	
	CreateDefaultRenderTarget(inWidth, inHeight);
}

bool SSDX11Device::CreateSwapChain(HWND windowHandle)
{
	HR(mDevice->CheckMultisampleQualityLevels(mSwapChainFormat, mSampleCount, &m4xMSAAQuality));

	//HR(mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&mDebug));

	IDXGIDevice* dxgiDevice = nullptr;
	HR(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdaptor = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdaptor));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	HR(dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2)));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		ID3D11Device1* Device1;
		ID3D11DeviceContext1* DeviceContext1;
		IDXGISwapChain1* SwapChain1;
		HR(mDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&Device1)));
		{
			(void)mDevice->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&DeviceContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = mBufferWidth;
		sd.Height = mBufferHeight;
		sd.Format = mSwapChainFormat;
		sd.SampleDesc.Count = mSampleCount;
		sd.SampleDesc.Quality = m4xMSAAQuality -1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		HR(dxgiFactory2->CreateSwapChainForHwnd(mDevice.Get(), windowHandle, &sd, nullptr, nullptr, &SwapChain1));

		{
			HR(SwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(mSwapChain.GetAddressOf())));
		}

		dxgiFactory2->Release();
	}

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdaptor);
	ReleaseCOM(dxgiFactory);

	return true;
}

bool SSDX11Device::CreateDefaultRenderTarget(int inWidth, int inHeight)
{
	check(inWidth > 0 && inHeight > 0);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = nullptr;
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}

	// Resize the swap chain and recreate the render target view.
	HR(mSwapChain->ResizeBuffers(0, inWidth, inHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = inWidth;
	depthStencilDesc.Height = inHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	depthStencilDesc.SampleDesc.Count = mSampleCount;
	depthStencilDesc.SampleDesc.Quality = m4xMSAAQuality - 1;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));

	SetDefaultRenderTargetAsCurrent();
	
	SSCameraManager::Get().SetCurrentCameraAspectRatio(static_cast<float>(inWidth) / static_cast<float>(inHeight));

	return true;
}

SSDepthRenderTargetTexture2D* SSDX11Device::CreateDepthRenderTargetTexture2D(const UINT InWidth, const UINT InHeight, DXGI_FORMAT eFormat)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	ID3D11Texture2D* DepthTexture = nullptr;

	depthStencilDesc.Width = InWidth;
	depthStencilDesc.Height = InHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = eFormat;

	// Use 4X MSAA? --must match swap chain MSAA values.
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, &DepthTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	ID3D11DepthStencilView* DepthStencilView = nullptr;

	depthStencilViewDesc.Format = eFormat;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(mDevice->CreateDepthStencilView(DepthTexture, &depthStencilViewDesc, &DepthStencilView));

	return new SSDepthRenderTargetTexture2D(InWidth, InHeight, eFormat, DepthTexture, DepthStencilView);
}

std::tuple<ID3D11Texture2D*, ID3D11DepthStencilView*>
SSDX11Device::InternalCreateDepthRenderTarget(const UINT InWidth, const UINT InHeight, DXGI_FORMAT eFormat)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	ID3D11Texture2D* DepthTexture = nullptr;

	depthStencilDesc.Width = InWidth;
	depthStencilDesc.Height = InHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = eFormat;

	// Use 4X MSAA? --must match swap chain MSAA values.
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, &DepthTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	ID3D11DepthStencilView* DepthStencilView = nullptr;

	depthStencilViewDesc.Format = eFormat;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(mDevice->CreateDepthStencilView(DepthTexture, &depthStencilViewDesc, &DepthStencilView));

	return std::tuple<ID3D11Texture2D*, ID3D11DepthStencilView*>(DepthTexture, DepthStencilView);
}

void SSDX11Device::ResizeDepthRenderTargetTexture2D(SSDepthRenderTargetTexture2D* InDepthRT, const UINT InWidth, const UINT InHeight)
{
	InDepthRT->Destroy();

	auto Result = InternalCreateDepthRenderTarget(InWidth, InHeight, InDepthRT->GetTextureFormat());

	InDepthRT->mTexturePtr = std::get<0>(Result);
	InDepthRT->mDepthStencilView = std::get<1>(Result);
}

void SSDX11Device::SetCurrentRenderTarget(SSDX11RenderTarget* InRenderTarget)
{
	const int RTCount = InRenderTarget->GetCount();

	D3D11_VIEWPORT Viewport = InRenderTarget->GetViewport();

	ID3D11RenderTargetView* RenderTargets[4]{ nullptr };

	for (UINT i = 0; i < InRenderTarget->GetCount(); ++i)
	{
		RenderTargets[i] = InRenderTarget->GetRenderTargetView(i);
	}

	if (InRenderTarget->IsDepthExist())
	{
		ID3D11DepthStencilView* depthStencil = InRenderTarget->GetDepthStencilView();

		mDeviceContext->OMSetRenderTargets(RTCount, RenderTargets, depthStencil);

		this->GetDeviceContext()->RSSetViewports(1, &(InRenderTarget->GetViewport()));
	}
	else
	{
		mDeviceContext->OMSetRenderTargets(RTCount, RenderTargets, nullptr);

		mDeviceContext->RSSetViewports(1, &(InRenderTarget->GetViewport()));
	}
}

SSDX11ConstantBuffer* SSDX11Device::CreateConstantBuffer(const UINT InBufferSize, const UINT InSlotIndex, std::string InName)
{
	D3D11_BUFFER_DESC BufferDesc{};

	// now create constant buffer
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	BufferDesc.ByteWidth = InBufferSize;
	
	ID3D11Buffer* BufferPtr = nullptr;
	HR(mDevice->CreateBuffer(&BufferDesc, nullptr, &BufferPtr));

	return new SSDX11ConstantBuffer(BufferPtr, InSlotIndex, InBufferSize, InName);
}

ID3D11VertexShader* SSDX11Device::GetBoundVertexShader()
{
	ID3D11VertexShader* VertexShader = nullptr;
	mDeviceContext->VSGetShader(&VertexShader, nullptr, nullptr);
	return VertexShader;
}

ID3D11PixelShader* SSDX11Device::GetBoundPixelShader()
{
	ID3D11PixelShader* PixelShader = nullptr;
	
	mDeviceContext->PSGetShader(&PixelShader, nullptr, nullptr);
	return PixelShader;
}

ID3D11Buffer* SSDX11Device::GetBoundConstantBufferVS(UINT slot)
{
	ID3D11Buffer* ConstantBuffer = nullptr;
	mDeviceContext->VSGetConstantBuffers(slot, 1, &ConstantBuffer);
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
	}
	return ConstantBuffer;
}

ID3D11Buffer* SSDX11Device::GetBoundConstantBufferPS(UINT slot)
{
	ID3D11Buffer* ConstantBuffer = nullptr;
	mDeviceContext->PSGetConstantBuffers(slot, 1, &ConstantBuffer);
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
	}
	return ConstantBuffer;
}


SSDX11PixelShader* SSDX11Device::CompilePixelShaderFromFile(std::wstring& Path)
{
	ID3D10Blob* errorMsg = nullptr;
	ID3DBlob* ShaderBlob = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;

	check(std::filesystem::exists(Path));

	std::vector<D3D_SHADER_MACRO> macros;

	// null terminate , otherwise crashes
	macros.push_back({ nullptr,nullptr });

	D3DCompileFromFile(Path.c_str(), macros.data(), nullptr, "PSMain", "ps_5_0", 0, 0, &ShaderBlob, &errorMsg);

	if (errorMsg != nullptr)
	{
		PrintCompileError(errorMsg);
		return nullptr;
	}
	
	HR(mDevice->CreatePixelShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), nullptr, &PixelShader));

	// @constant buffer reflection
	ID3D11ShaderReflection* PixelShaderReflection = nullptr;
	HR(D3DReflect(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&PixelShaderReflection));
	
	return nullptr;
}

void SSDX11Device::PrintCompileError(ID3DBlob* errorMessage)
{
	auto buffSize = errorMessage->GetBufferSize();

	const char* errorMsgPtr = (const char*)errorMessage->GetBufferPointer();

	char* compileErrBuffer = new char[buffSize + 1];

	strcpy_s(compileErrBuffer, buffSize, errorMsgPtr);

	OutputDebugStringA("============= Shader Compile Error =============\n");
	OutputDebugStringA(compileErrBuffer);
	OutputDebugStringA("============= Shader Compile Error =============\n");

	delete[] compileErrBuffer;
}
