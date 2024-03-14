#pragma once
#include "Job.h"

class JobQueue
{
public:
	void Push(shared_ptr<Job> job);
	shared_ptr<Job> Pop();
	void Flush();

private:
	USE_LOCK;
	queue<shared_ptr<Job>> _jobs;
};
