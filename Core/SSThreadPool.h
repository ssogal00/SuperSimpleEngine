

	
#pragma once
#include <functional>
#include <queue>
#include <atomic>
#include "Windows.h"

class SSThreadJobItem
{
public:
	virtual void DoWork() = 0;
};

class CORE_API SSThreadPool
{
public:		
	SSThreadPool();
	~SSThreadPool();

	void EnqueueJob(std::function<void()> InJob );

	CRITICAL_SECTION& GetJobQueueCriticalSection() { return mJobQueueCriticalSection; }

	friend DWORD ThreadPoolWorkerThreadProc(LPVOID lpParam);

private:
	std::atomic<bool> mShutdown{ false };

	std::queue<std::function<void()>> mJobQueue;

	CRITICAL_SECTION mJobQueueCriticalSection;

	CONDITION_VARIABLE mJobQueueConditionVariable;

	int mCoreCount = 0;
		
	HANDLE* mThreadHandleArray;
	DWORD* mThreadIDs;
};
