
#include "Core.h"
#include "SSEngine12.h"



void SSEngine12::Initialize(HWND windowHandle)
{
	mWindowHandle = windowHandle;
	
	CreateDevice();

}

bool SSEngine12::CreateDevice()
{
	if (mUseWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;

		HR(mFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		HR(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)));
	}
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;

		GetHardwareAdapter(mFactory.Get(), &hardwareAdapter);

		HR(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)));
	}

	return true;
}

bool SSEngine12::CreateSwapChain()
{
		
}


void SSEngine12::LoadPipeline()
{
	UINT dxgiFactoryFlags = 0;

#if defined(DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif		

	HR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));

	CreateDevice();

	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	HR(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.BufferCount = 1;
	

	

	ComPtr<IDXGISwapChain1> swapChain;

	HR(mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), mWindowHandle, &swapChainDesc, nullptr, nullptr, &swapChain));



}


void SSEngine12::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}