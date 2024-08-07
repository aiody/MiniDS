#pragma once

class CPUUsage {
public:
    CPUUsage() {
        SYSTEM_INFO sysInfo;
        ::GetSystemInfo(&sysInfo);

        _prevSysIdleTime = { 0 };
        _prevSysKernelTime = { 0 };
        _prevSysUserTime = { 0 };
        _prevProcKernelTime = { 0 };
        _prevProcUserTime = { 0 };
    }

    void Update()
    {
        FILETIME sysIdleTime, sysKernelTime, sysUserTime;
        FILETIME procCreationTime, procExitTime, procKernelTime, procUserTime;

        if (::GetSystemTimes(&sysIdleTime, &sysKernelTime, &sysUserTime)
            && ::GetProcessTimes(::GetCurrentProcess(), &procCreationTime, &procExitTime, &procKernelTime, &procUserTime))
        {
            uint64 sysIdleDelta = SubtractTimes(sysIdleTime, _prevSysIdleTime);
            uint64 sysKernelDelta = SubtractTimes(sysKernelTime, _prevSysKernelTime);
            uint64 sysUserDelta = SubtractTimes(sysUserTime, _prevSysUserTime);
            uint64 sysTotal = sysKernelDelta + sysUserDelta;

            uint64 procKernelDelta = SubtractTimes(procKernelTime, _prevProcKernelTime);
            uint64 procUserDelta = SubtractTimes(procUserTime, _prevProcUserTime);
            uint64 procTotal = procKernelDelta + procUserDelta;

            if (sysTotal > 0)
            {
                _sysCPUUsage = (1.0 - (sysIdleDelta * 1.0 / sysTotal)) * 100.0;
                _procCPUUsage = (procTotal * 100.f / sysTotal);
            }
            else
            {
                _sysCPUUsage = 0.0f;
                _procCPUUsage = 0.0f;
            }

            _prevSysIdleTime = sysIdleTime;
            _prevSysKernelTime = sysKernelTime;
            _prevSysUserTime = sysUserTime;
            _prevProcKernelTime = procKernelTime;
            _prevProcUserTime = procUserTime;
        }
        else
        {
            _sysCPUUsage = 0.0f;
            _procCPUUsage = 0.0f;
        }
    }

    double GetSystemCPUUsage() { return _sysCPUUsage; }
    double GetProcessCPUUsage() { return _procCPUUsage; }

private:
    uint64 SubtractTimes(FILETIME& newTime, FILETIME& oldTime)
    {
        ULARGE_INTEGER a, b;
        a.LowPart = newTime.dwLowDateTime;
        a.HighPart = newTime.dwHighDateTime;
        b.LowPart = oldTime.dwLowDateTime;
        b.HighPart = oldTime.dwHighDateTime;
        return a.QuadPart - b.QuadPart;
    }

private:
    double _sysCPUUsage = 0.0f;
    double _procCPUUsage = 0.0f;

    FILETIME _prevSysIdleTime;
    FILETIME _prevSysKernelTime;
    FILETIME _prevSysUserTime;
    FILETIME _prevProcKernelTime;
    FILETIME _prevProcUserTime;
};


class MonitoringInfo
{
public:
    MonitoringInfo();
    ~MonitoringInfo();

    void Init();
	void SetService(shared_ptr<Service> service) { _service = service; }
	shared_ptr<Service> GetService() { return _service; }

	DWORDLONG	GetTotalVirtualMemory();
	DWORDLONG	GetVirtualMemoryCurrentlyUsed();
	SIZE_T		GetVirtualMemoryUsedByMe();
	DWORDLONG	GetTotalPhysicalMemory();
	DWORDLONG	GetPhysicalMemoryCurrentlyUsed();
	SIZE_T		GetPhysicalMemoryUsedByMe();
    void        ComputeCPUUsage();
	double		GetCPUCurrentyUsed();
	double		GetCPUUsedByMe();
    string      GetServerStartTime() { return _serverStartTimeString; }
    void        AddTransferredPacketCount();
    uint64      GetTransferredPacketCount();

private:
    USE_LOCK;
	shared_ptr<Service> _service;
    CPUUsage* _cpuUsage;
    string _serverStartTimeString;
    uint64 _transferredPacketCount;
};
