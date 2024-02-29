#include "pch.h"
#include "Global.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "ClientPacketHandler.h"

ThreadManager* gThreadManager = nullptr;

class Global
{
public:
	Global()
	{
		gThreadManager = new ThreadManager();
		SocketUtils::Init();
		ClientPacketHandler::Init();
	}

	~Global()
	{
		delete gThreadManager;
		SocketUtils::Clear();
	}
} GGlobal; // 전역 객체
