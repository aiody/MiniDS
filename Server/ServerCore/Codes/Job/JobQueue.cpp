#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

void JobQueue::Push(shared_ptr<Job> job)
{
	const int32 prevCount = _jobCount.fetch_add(1);
	
	{
		WRITE_LOCK;
		_jobs.push(job);
	}

	// 이 jobQueue에 Job을 처음 넣은 쓰레드가 실행까지 담당
	if (prevCount == 0)
	{
		// 현재 쓰레드가 실행중인 JobQueue가 없으면 실행
		if (LCurrentJobQueue == nullptr)
			Execute();
		else // 여유 있는 다른 쓰레드가 실행하도록 GlobalQueue에 넘김
			gGlobalQueue->Push(shared_from_this());
	}
}

void JobQueue::Execute()
{
	LCurrentJobQueue = this;

	while (true)
	{
		vector<shared_ptr<Job>> jobs;
		// 실행 가능한 job을 모두 꺼내기
		{
			WRITE_LOCK;
			while (_jobs.empty() == false)
			{
				jobs.push_back(_jobs.front());
				_jobs.pop();
			}
		}

		const int32 jobCount = static_cast<int32>(jobs.size());
		for (int32 i = 0; i < jobCount; i++)
			jobs[i]->Execute();

		// 남은 일감이 없으면 종료
		if (_jobCount.fetch_sub(jobCount) == jobCount)
		{
			LCurrentJobQueue = nullptr;
			break;
		}

		// 쓰레드를 너무 오래 이 단계에 붙잡고 있지 않도록 조절
		const uint64 now = ::GetTickCount64();
		if (now > LEndTickCount)
		{
			// 여유 있는 다른 쓰레드가 실행하도록 GlobalQueue에 넘김
			gGlobalQueue->Push(shared_from_this());
			LCurrentJobQueue = nullptr;
			break;
		}
	}
}
