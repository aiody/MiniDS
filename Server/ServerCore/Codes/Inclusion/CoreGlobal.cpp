#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "GlobalQueue.h"
#include "JobTimer.h"

ThreadManager*	gThreadManager = nullptr;
GlobalQueue*	gGlobalQueue = nullptr;
JobTimer*		gJobTimer = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		SocketUtils::Init();
		gThreadManager = new ThreadManager();
		gGlobalQueue = new GlobalQueue();
		gJobTimer = new JobTimer();
	}

	~CoreGlobal()
	{
		SocketUtils::Clear();
		delete gThreadManager;
		delete gGlobalQueue;
		delete gJobTimer;
	}
} GCoreGlobal; // 전역 객체
