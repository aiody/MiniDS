#include "pch.h"
#include "Global.h"
#include "MonitoringInfo.h"

MonitoringInfo* gMonitoringInfo = nullptr;

class Global
{
public:
	Global()
	{
		gMonitoringInfo = new MonitoringInfo();
		gMonitoringInfo->Init();
	}
	~Global()
	{
		delete gMonitoringInfo;
	}
} GGlobal;
