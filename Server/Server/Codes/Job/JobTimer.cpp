#include "pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

void JobTimer::Reserve(uint64 tickAfter, shared_ptr<Job> job)
{
	const uint64 executeTick = ::GetTickCount64() + tickAfter;

	WRITE_LOCK;
	_items.push(TimerItem{ executeTick, job });
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
		gJobQueue->Push(item.job);
	}

	_distributing.store(false);
}
