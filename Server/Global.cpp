#include "pch.h"
#include "Global.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "ClientPacketHandler.h"
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
		gThreadManager = new ThreadManager();
		gJobQueue = new JobQueue();
		gJobTimer = new JobTimer();
		
		SocketUtils::Init();
		ClientPacketHandler::Init();
	}

	~Global()
	{
		delete gThreadManager;
		delete gJobQueue;
		delete gJobTimer;
		SocketUtils::Clear();
	}
} GGlobal; // 전역 객체
