#pragma once
#include "Job.h"
#include "JobQueue.h"

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	shared_ptr<Job> job = nullptr;
};

class JobTimer
{
public:
	void Reserve(uint64 tickAfter, shared_ptr<Job> job);
	void Distribute(uint64 now);

private:
	USE_LOCK;
	priority_queue<TimerItem> _items;
	atomic<bool> _distributing = false;
};
