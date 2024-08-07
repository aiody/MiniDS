#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "JobQueue.h"
#include "JobTimer.h"

ThreadManager* gThreadManager = nullptr;
JobQueue* gJobQueue = nullptr;
JobTimer* gJobTimer = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		SocketUtils::Init();
		
		gThreadManager = new ThreadManager();
		gJobQueue = new JobQueue();
		gJobTimer = new JobTimer();
	}

	~CoreGlobal()
	{
		SocketUtils::Clear();
		delete gThreadManager;
		delete gJobQueue;
		delete gJobTimer;
	}
} GCoreGlobal; // 전역 객체
