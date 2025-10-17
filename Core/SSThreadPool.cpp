
#include "SSCore.h"
#include "SSThreadPool.h"

DWORD ThreadPoolWorkerThreadProc(LPVOID lpParam)
{
	HANDLE eventHandle = (HANDLE)lpParam;

	while (true)
	{
		// Wait for the event to be signaled
		WaitForSingleObject(eventHandle, INFINITE);

		// Process jobs from the job queue
		// (Implementation of job processing goes here)
	}
}

SSThreadPool::SSThreadPool()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	mCoreCount = std::min<int>(sysInfo.dwNumberOfProcessors, 4);
	check(mCoreCount > 0);
	
	mThreadPoolEventHandleArray = new HANDLE[mCoreCount];
	mThreadHandleArray = new HANDLE[mCoreCount];

	for (int i = 0; i < mCoreCount; ++i)
	{
		mThreadPoolEventHandleArray[i] = CreateEvent(
			NULL,               // default security attributes
			FALSE,              // auto-reset event object
			FALSE,              // initial state is nonsignaled
			NULL);              // unnamed object

		mThreadHandleArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			&ThreadPoolWorkerThreadProc,       // thread function name
			(LPVOID)mThreadPoolEventHandleArray[i],          // argument to thread function 
			0,                      // use default creation flags 
			NULL);   // returns the thread identifier
	}
}



void SSThreadPool::EnqueueJob(std::function<void()> InJob)
{
	EnterCriticalSection(&mJobQueueCriticalSection);
	mJobQueue.push(InJob);
	LeaveCriticalSection(&mJobQueueCriticalSection);

}



SSThreadPool::~SSThreadPool()
{
	for (int i = 0; i < mCoreCount; ++i)
	{
		CloseHandle(mThreadPoolEventHandleArray[i]);
	}
	delete[] mThreadPoolEventHandleArray;
}