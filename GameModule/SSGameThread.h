#pragma once
#include "SSGameModule.h"
#include "SSTimer.h"

// game thread is the same as main thread
class GAMEMODULE_API SSGameThread
{
public:	
	SSGameThread(DWORD gameThreadId);

	static bool IsInGameThread();

	void Tick();

	HANDLE GetGameThreadEventHandle() { return mGameThreadDoneEventHandle; }	

	void WaitForGameThread(const DWORD WaitTime = INFINITE);

protected:
	void Start(DWORD gameThreadId);

	DWORD Run();

	static DWORD Run(LPVOID param);

	static DWORD mGameThreadId;

	HANDLE mThreadHandle = nullptr;

	HANDLE mGameThreadDoneEventHandle = nullptr;

	bool bRequestExist = false;

	unsigned long long mFrameCount = 0;

	double mFrameSeconds=0;

	SSGameTimer mGameThreadTimer;

	class SSGameScene* mCurrentGameScene = nullptr;
};