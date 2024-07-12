#include "pch.h"
#include "JobQueue.h"

void JobQueue::Push(shared_ptr<Job> job)
{
	WRITE_LOCK;
	_jobs.push(job);
}

shared_ptr<Job> JobQueue::Pop()
{
	WRITE_LOCK;
	if (_jobs.empty())
		return nullptr;

	shared_ptr<Job> job = _jobs.front();
	_jobs.pop();
	return job;
}

void JobQueue::Flush()
{
	while (true)
	{
		shared_ptr<Job> job = Pop();
		if (job == nullptr)
			break;

		job->Execute();
	}
}
