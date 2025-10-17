
#include "SSMemoryAllocator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <deque>
#include "Test.h"
#include "SSTimer.h"
#include "SSThreadPool.h"

using namespace std::chrono_literals;


template<class T>
class TSizeofClass
{
public:
	static constexpr int Size = sizeof(T);
};

class FourByteClass
{
public:
	FourByteClass(unsigned int InMember) : mMember(InMember)
	{
		//std::cout << "FourByteClass() Constructor"<<mMember << std::endl;
	}
	~FourByteClass()
	{
		//std::cout << "~FourByteClass() Destructor"<<mMember << std::endl;
	}

	
	unsigned int mMember = 0;
	unsigned int mMember2 = 0;
};


class FVirtualClass
{
public:
	virtual void Printinfo();
};

class FEmptyClass
{

};



int main()
{
	SSThreadPool TestPool{};
	
	for (int i = 0; i < 10; ++i)
	{
		TestPool.EnqueueJob([]()
		{
				int WaitMS = (std::rand() % 10) * 100 ;
				::Sleep(WaitMS);
			DWORD ThreadId = ::GetCurrentThreadId();
			std::cout << "ThreadID : " << ThreadId <<" Waited : "<<WaitMS << " Hello from thread pool job!" << std::endl;
		});
	}
	
	::Sleep(2000); // Wait for jobs to finish

	return 0;
}