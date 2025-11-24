
#include "SSCore.h"
#include "SSRenderer.h"
#include "SSRenderingThread.h"
#include "Windows.h"
#include "SSTimer.h"
#include "SSRenderingObjectManager.h"

SSRenderingThread::SSRenderingThread(SSRenderer* pRenderer)
{
	mRenderer = pRenderer;
}

void SSRenderingThread::Start(HWND handle)
{
    mWindowHandle = handle;
	mThreadHandle = CreateThread(nullptr, 0, &SSRenderingThread::Run, this, 0, &mRenderingThreadId);
	InitializeCriticalSection(&mCriticalSection);
	mEventName = "RenderingThreadDoneEvent";
	mRenderingDoneEventHandle = CreateEvent(nullptr, false, false, mEventName);
	mGameThreadWaitHandle = CreateEvent(nullptr, false, false, "GameThreadWaitHandle");
}

void SSRenderingThread::SetRenderer(class SSRenderer* renderer)
{
	mRenderer = renderer;
}

void SSRenderingThread::WaitForRenderingThread(const DWORD WaitTime)
{
	WaitForSingleObject(mRenderingDoneEventHandle, WaitTime);
}

// called from engine when game thread done
void SSRenderingThread::SetGameThreadDone()
{
	SetEvent(mGameThreadWaitHandle);
}

DWORD SSRenderingThread::Run()
{
	// init renderer
	mRenderer->Initialize(mWindowHandle);	

	bIsRunning = true;

	SSGameTimer renderingThreadTimer;
	renderingThreadTimer.Tick();

	while (1)
	{
		// wait for game thread done
		DWORD Result = WaitForSingleObject(mGameThreadWaitHandle, 100);
		if (Result != WAIT_OBJECT_0)
		{
			// game thread wait time expired 
		}

		// consume command queue
		{
			EnterCriticalSection(&mCriticalSection);
			if (mCommandQueue.empty() == false)
			{
				for (auto iter = mCommandQueue.begin(); iter != mCommandQueue.end(); iter++)
				{
					(*iter)();
				}
				mCommandQueue.clear();
			}
			LeaveCriticalSection(&mCriticalSection);
		}
		//
		renderingThreadTimer.Tick();

		// Tick Rendering Thread

		if (mRenderer)
		{
			SSRenderingObjectManager::Get().Tick(renderingThreadTimer.GetDeltaTime());
			// sync gamethread object <-> rendering thread object
			mRenderer->UpdateRenderingObjects();
			mRenderer->DrawScene();
		}

		SetEvent(mRenderingDoneEventHandle);

		if (bRequestExit)
		{
			if(mRenderer)
			{
				mRenderer->Shutdown();
				delete mRenderer;
				mRenderer = nullptr;
			}
			return 0;
		}
	}	
}

DWORD SSRenderingThread::Run(LPVOID param)
{
	SSRenderingThread* threadInstance = (SSRenderingThread*)param;
	return threadInstance->Run();
}

void SSRenderingThread::Join()
{
	DWORD waitResult = WaitForSingleObject(mThreadHandle, INFINITE);
	check(waitResult == WAIT_OBJECT_0);
}

bool SSRenderingThread::IsInRenderingThread()
{
    if(mRenderingThreadId == GetCurrentThreadId())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SSRenderingThread::ExecuteInRenderingThread(std::function<void()>&& lambdaFunction)
{
    if(IsInRenderingThread())
    {
        lambdaFunction();
    }
    else
    {        
		EnterCriticalSection(&mCriticalSection);
        mCommandQueue.push_back(std::move(lambdaFunction));
		LeaveCriticalSection(&mCriticalSection);
    }    
}

void SSRenderingThread::PauseRendering()
{
	if(mRenderer)
	{
		mRenderer->PauseRendering();
	}
}

void SSRenderingThread::ResumeRendering()
{
	if(mRenderer)
	{
		mRenderer->ResumeRendering();
	}
}



DWORD SSRenderingThread::mRenderingThreadId = 0;