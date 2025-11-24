
#include "SSGameModule.h"
#include "SSGameThread.h"
#include "SSGameObjectManager.h"
#include "Windows.h"
#include "SSGameWindow.h"
#include "SSCameraManager.h"
#include "SSCubeScene.h"

SSGameThread::SSGameThread(DWORD gameThreadId)
{
	Start(gameThreadId);
}


bool SSGameThread::IsInGameThread()
{
	if (mGameThreadId== GetCurrentThreadId())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SSGameThread::Start(DWORD gameThreadId)
{	
	mGameThreadId = gameThreadId;
	mGameThreadDoneEventHandle = CreateEvent(nullptr, false, false, "GameThreadEventHandle");

	mCurrentGameScene = new SSCubeScene();
	mCurrentGameScene->InitializeScene();
}

void SSGameThread::Tick()
{
	// do game thread work

	// handle window message
	SSGameWindow::GetPtr()->HandleMessage();

	float tickTime = mGameThreadTimer.GetDeltaTime();
	// frametime in milliseconds
	DWORD frameMilliseconds = (DWORD) (tickTime * 1000);
	// gamethread 120fps
	float frameCap = (1.0f / 120.f) * 1000;

	if(frameMilliseconds < frameCap)
    {
	    DWORD sleeptime = (DWORD) (frameCap - frameMilliseconds);
        Sleep(sleeptime);
    }

    mGameThreadTimer.Tick();

	SSCameraManager::Get().Tick(mGameThreadTimer.GetDeltaTime());

	if (mCurrentGameScene)
	{
		mCurrentGameScene->Tick(mGameThreadTimer.GetDeltaTime());
	}

	SSGameObjectManager::GetPtr()->Tick(mGameThreadTimer.GetDeltaTime());

	// set event
	SetEvent(mGameThreadDoneEventHandle);
}

void SSGameThread::WaitForGameThread(const DWORD WaitTime)
{
	WaitForSingleObject(mGameThreadDoneEventHandle, WaitTime);	
}


DWORD SSGameThread::mGameThreadId = 0;