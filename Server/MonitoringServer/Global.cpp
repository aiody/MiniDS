#include "pch.h"
#include "Global.h"
#include "JobQueue.h"

shared_ptr<JobQueue> gJobQueue = nullptr;

class Global
{
public:
	Global()
	{
		gJobQueue = make_shared<JobQueue>();
	}
	~Global()
	{
		gJobQueue = nullptr;
	}
} GGlobal;
