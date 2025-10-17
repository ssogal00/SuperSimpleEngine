

	
#pragma once
#include <functional>
#include <queue>
#include "Windows.h"

class SSThreadJobItem
{
public:
	virtual void DoWork() = 0;
};

class SSThreadPool
{
public:		
	SSThreadPool();
	~SSThreadPool();

	void EnqueueJob(std::function<void()> InJob );

private:
	std::queue<std::function<void()>> mJobQueue;
	CRITICAL_SECTION mJobQueueCriticalSection;

	int mCoreCount = 0;
	HANDLE* mThreadPoolEventHandleArray;
	HANDLE* mThreadHandleArray;
};
