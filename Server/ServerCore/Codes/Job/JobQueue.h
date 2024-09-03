#pragma once
#include "Job.h"

class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	void Push(shared_ptr<Job> job);
	void Execute();

private:
	USE_LOCK;
	queue<shared_ptr<Job>>	_jobs;
	atomic<int32>			_jobCount = 0;
};
