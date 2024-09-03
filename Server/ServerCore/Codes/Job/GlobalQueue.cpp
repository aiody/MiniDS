#include "pch.h"
#include "GlobalQueue.h"

void GlobalQueue::Push(shared_ptr<JobQueue> jobQueue)
{
	WRITE_LOCK;
	_jobQueues.push(jobQueue);
}

shared_ptr<JobQueue> GlobalQueue::Pop()
{
	WRITE_LOCK;

	if (_jobQueues.empty())
		return nullptr;

	shared_ptr<JobQueue> jobQueue = _jobQueues.front();
	_jobQueues.pop();

	return jobQueue;
}
