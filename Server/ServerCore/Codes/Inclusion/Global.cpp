#include "pch.h"
#include "Global.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "JobQueue.h"
#include "JobTimer.h"

ThreadManager* gThreadManager = nullptr;
JobQueue* gJobQueue = nullptr;
JobTimer* gJobTimer = nullptr;

class Global
{
public:
	Global()
	{
		SocketUtils::Init();
		
		gThreadManager = new ThreadManager();
		gJobQueue = new JobQueue();
		gJobTimer = new JobTimer();
	}

	~Global()
	{
		SocketUtils::Clear();
		delete gThreadManager;
		delete gJobQueue;
		delete gJobTimer;
	}
} GGlobal; // 전역 객체
