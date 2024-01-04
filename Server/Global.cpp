#include "pch.h"
#include "Global.h"
#include "SocketUtils.h"

class Global
{
public:
	Global()
	{
		SocketUtils::Init();
	}

	~Global()
	{
		SocketUtils::Clear();
	}
} GGlobal; // 전역 객체
