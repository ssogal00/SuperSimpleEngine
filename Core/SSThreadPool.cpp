
#include "SSCore.h"
#include "SSThreadPool.h"

DWORD ThreadPoolWorkerThreadProc(LPVOID lpParam)
{
	SSThreadPool* threadPool = (SSThreadPool*)lpParam;

	while (!threadPool->mShutdown)
	{
		std::function<void()> job;
		EnterCriticalSection(&threadPool->mJobQueueCriticalSection);

		while (!threadPool->mShutdown && threadPool->mJobQueue.empty())
		{
			SleepConditionVariableCS(&threadPool->mJobQueueConditionVariable, &threadPool->mJobQueueCriticalSection, INFINITE);
		}
		
		if (!threadPool->mShutdown)
		{
			job = threadPool->mJobQueue.front();
		}

		LeaveCriticalSection(&threadPool->mJobQueueCriticalSection);

		if (!threadPool->mShutdown)
		{			
			threadPool->mJobQueue.pop();
			job();
		}
	}

	return 0;
}

SSThreadPool::SSThreadPool()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	mCoreCount = std::min<int>(sysInfo.dwNumberOfProcessors, 4);
	check(mCoreCount > 0);
		
	mThreadHandleArray = new HANDLE[mCoreCount];
	mThreadIDs = new DWORD[mCoreCount];

	InitializeCriticalSection(&mJobQueueCriticalSection);
	InitializeConditionVariable(&mJobQueueConditionVariable);

	for (int i = 0; i < mCoreCount; ++i)
	{
		mThreadHandleArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			&ThreadPoolWorkerThreadProc,       // thread function name
			(LPVOID)this,          // argument to thread function 
			0,                      // use default creation flags 
			&mThreadIDs[i]);   // returns the thread identifier
	}
}

void SSThreadPool::EnqueueJob(std::function<void()> InJob)
{
	EnterCriticalSection(&mJobQueueCriticalSection);
	mJobQueue.push(InJob);
	LeaveCriticalSection(&mJobQueueCriticalSection);

	WakeConditionVariable(&mJobQueueConditionVariable);
}

SSThreadPool::~SSThreadPool()
{
	EnterCriticalSection(&mJobQueueCriticalSection);
	mShutdown = true;
	LeaveCriticalSection(&mJobQueueCriticalSection);

	WakeAllConditionVariable(&mJobQueueConditionVariable);

	WaitForMultipleObjects(mCoreCount, mThreadHandleArray, TRUE, INFINITE);

	for (int i = 0; i < mCoreCount; ++i)
	{		
		CloseHandle(mThreadHandleArray[i]);
	}

	DeleteCriticalSection(&mJobQueueCriticalSection);
}