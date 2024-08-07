#include "pch.h"
#include "MonitoringInfo.h"
#include "psapi.h"
#include <iomanip>
#include <ctime>
#include <sstream>

MonitoringInfo::MonitoringInfo()
{
	_cpuUsage = new CPUUsage();
}

MonitoringInfo::~MonitoringInfo()
{
	delete _cpuUsage;
}

void MonitoringInfo::Init()
{
	// get server start time
	{
		auto t = ::time(nullptr);
		tm tm;
		::localtime_s(&tm, &t);
		ostringstream oss;
		oss << ::put_time(&tm, "%Y-%m-%d %H:%M:%S");
		_serverStartTimeString = oss.str();
	}
}

DWORDLONG MonitoringInfo::GetTotalVirtualMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	::GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	return totalVirtualMem;
}

DWORDLONG MonitoringInfo::GetVirtualMemoryCurrentlyUsed()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	::GlobalMemoryStatusEx(&memInfo);
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	return virtualMemUsed;
}

SIZE_T MonitoringInfo::GetVirtualMemoryUsedByMe()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(::GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	return virtualMemUsedByMe;
}

DWORDLONG MonitoringInfo::GetTotalPhysicalMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	::GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	return totalPhysMem;
}

DWORDLONG MonitoringInfo::GetPhysicalMemoryCurrentlyUsed()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	::GlobalMemoryStatusEx(&memInfo);
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	return physMemUsed;
}

SIZE_T MonitoringInfo::GetPhysicalMemoryUsedByMe()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	::GetProcessMemoryInfo(::GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	return physMemUsedByMe;
}

void MonitoringInfo::ComputeCPUUsage()
{
	_cpuUsage->Update();
}

double MonitoringInfo::GetCPUCurrentyUsed()
{
	return _cpuUsage->GetSystemCPUUsage();
}

double MonitoringInfo::GetCPUUsedByMe()
{
	return _cpuUsage->GetProcessCPUUsage();
}

void MonitoringInfo::AddTransferredPacketCount()
{
	WRITE_LOCK;
	_transferredPacketCount++;
}

uint64 MonitoringInfo::GetTransferredPacketCount()
{
	WRITE_LOCK;
	uint64 tpc = _transferredPacketCount;
	_transferredPacketCount = 0;
	return tpc;
}
