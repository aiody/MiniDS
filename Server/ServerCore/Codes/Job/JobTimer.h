#pragma once
#include "Job.h"
#include "JobQueue.h"

struct JobData
{
	JobData(weak_ptr<JobQueue> owner, shared_ptr<Job> job) : owner(owner), job(job) { }

	weak_ptr<JobQueue> owner;
	shared_ptr<Job> job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	JobData* jobData = nullptr;
};

class JobTimer
{
public:
	void Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, shared_ptr<Job> job);
	void Distribute(uint64 now);
	void Clear();

private:
	USE_LOCK;
	priority_queue<TimerItem> _items;
	atomic<bool> _distributing = false;
};
