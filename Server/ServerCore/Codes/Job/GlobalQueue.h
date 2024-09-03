#pragma once

class GlobalQueue
{
public:
	void Push(shared_ptr<JobQueue> jobQueue);
	shared_ptr<JobQueue> Pop();

private:
	USE_LOCK;
	queue<shared_ptr<JobQueue>> _jobQueues;
};
