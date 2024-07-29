#include "pch.h"
#include "MonitoringInfo.h"
#include "psapi.h"

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

double MonitoringInfo::GetCPUCurrentyUsed()
{
	return _cpuUsage.GetSystemCPUUsage();
}

double MonitoringInfo::GetCPUUsedByMe()
{
	return _cpuUsage.GetProcessCPUUsage();
}
