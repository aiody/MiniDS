#include "pch.h"
#include "Global.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "ClientPacketHandler.h"
#include "JobQueue.h"

ThreadManager* gThreadManager = nullptr;
JobQueue* gJobQueue = nullptr;

class Global
{
public:
	Global()
	{
		gThreadManager = new ThreadManager();
		gJobQueue = new JobQueue();
		
		SocketUtils::Init();
		ClientPacketHandler::Init();
	}

	~Global()
	{
		delete gThreadManager;
		delete gJobQueue;
		SocketUtils::Clear();
	}
} GGlobal; // 전역 객체
