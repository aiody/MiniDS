#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
	InitTLS(); // for main thread
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	lock_guard<mutex> lockGuard(_lock);

	_threads.push_back(thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}

	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}

uint32 ThreadManager::GetThreadCountToLaunch()
{
	uint32 numOfProcessor = ::thread::hardware_concurrency();
	cout << "[System] Logical processor count : " << numOfProcessor << endl;
	return numOfProcessor * 1.5f; // (NumberOfProcessor * 2) + 1 이 효율적이라고도 함.
}

uint32 ThreadManager::GetCurrentThreadCount()
{
	return _threads.size();
}
