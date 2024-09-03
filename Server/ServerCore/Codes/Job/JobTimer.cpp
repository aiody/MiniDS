#include "pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

void JobTimer::Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, shared_ptr<Job> job)
{
	const uint64 executeTick = ::GetTickCount64() + tickAfter;
	JobData* jobData = new JobData(owner, job);

	WRITE_LOCK;
	_items.push(TimerItem{ executeTick, jobData });
}

void JobTimer::Distribute(uint64 now)
{
	if (_distributing.exchange(true) == true)
		return;

	vector<TimerItem> items;
	{
		WRITE_LOCK;
		while (_items.empty() == false)
		{
			const TimerItem& item = _items.top();
			if (now < item.executeTick)
				break;

			items.push_back(item);
			_items.pop();
		}
	}

	for(TimerItem& item : items)
	{
		if (shared_ptr<JobQueue> owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job);

		delete item.jobData;
	}

	_distributing.store(false);
}

void JobTimer::Clear()
{
	WRITE_LOCK;
	while (_items.empty() == false)
	{
		const TimerItem& item = _items.top();
		delete item.jobData;
		_items.pop();
	}
}
