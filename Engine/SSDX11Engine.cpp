

#include "SSDX11Engine.h"
#include "SSDX11Renderer.h"
#include "SSCubeScene.h"
#include "SSGameObjectManager.h"
#include "SSRenderingObjectManager.h"
#include "SSSharedRenderData.h"
#include "SSGameWindow.h"
#include "SSRenderingThread.h"
#include "SSCameraManager.h"

bool SSDX11Engine::bInitialized = false;

SSDX11Engine* SSDX11Engine::mInstance = nullptr;

SSDX11Engine& SSDX11Engine::Get()
{
	if(mInstance == nullptr)
	{
		mInstance = new SSDX11Engine();
	}

	return (*mInstance);
}

SSDX11Engine* SSDX11Engine::GetPtr()
{
	if (mInstance == nullptr)
	{
		mInstance = new SSDX11Engine();
	}

	return mInstance;
}

void SSDX11Engine::Initialize(HWND windowHandle)
{
    mWindowHandle = windowHandle;	
		
	bInitialized = true;
}

void SSDX11Engine::ToggleGBufferDumpMode()
{
	bGbufferDump = !bGbufferDump;
}

void SSDX11Engine::Shutdown()
{
	if (mRenderingThread)
	{
		// wait for rendering thread		
		mRenderingThread->RequestExit();
		mRenderingThread->Join();

		delete mRenderingThread;
		mRenderingThread = nullptr;
	}

	if(mGameThread)
	{
		delete mGameThread;
		mGameThread = nullptr;
	}	
}

void SSDX11Engine::OnWindowResize(int newWidth, int newHeight)
{
	if (bInitialized)
	{
		if(mRenderer)
		{
			mRenderer->OnWindowResize(newWidth, newHeight);
		}

		SSCameraManager::Get().SetCurrentCameraAspectRatio(static_cast<float>(newWidth) / static_cast<float>(newHeight));
	}
}


void SSDX11Engine::EngineStart()
{
	// create renderer
	mRenderer = new SSDX11Renderer();

	mRenderer->SetWindowWidth(SSGameWindow::GetPtr()->GetWindowWidth());
	mRenderer->SetWindowHeight(SSGameWindow::GetPtr()->GetWindowHeight());

	// 
	mRenderingThread = new SSRenderingThread(mRenderer);
	
	// start game thread
	mGameThread = new SSGameThread(GetCurrentThreadId());
	
	// start rendering thread
	mRenderingThread->Start(SSGameWindow::GetPtr()->GetWindowHandle());

	Initialize(SSGameWindow::GetPtr()->GetWindowHandle());
}


void SSDX11Engine::Run()
{
	SSSharedRenderData::Get().Initialize();
	
	EngineStart();

	// Main message loop
	MSG msg = { 0 };
	while (!bRequestExit)
	{	
		// wait for rendering thread
		bool bDebuggerPresent = IsDebuggerPresent();

		if(bDebuggerPresent)
		{
			mRenderingThread->WaitForRenderingThread(INFINITE);
		}
		else
		{
			mRenderingThread->WaitForRenderingThread(100);
		}		

		// tick
		mGameThread->Tick();
		
		// this is awkward...
		SSRenderingObjectManager::Get().SetPendingObjects(SSGameObjectManager::Get().GetGameObjectMap());

		// notify rendering thread that game thread is done.
		mRenderingThread->SetGameThreadDone();		
	}

	Shutdown();
}


