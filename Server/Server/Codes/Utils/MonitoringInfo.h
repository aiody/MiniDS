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

    double GetSystemCPUUsage()
    {
        FILETIME idleTime, kernelTime, userTime;
        ::GetSystemTimes(&idleTime, &kernelTime, &userTime);

        uint64 idleDelta = SubtractTimes(idleTime, _prevSysIdleTime);
        uint64 kernelDelta = SubtractTimes(kernelTime, _prevSysKernelTime);
        uint64 userDelta = SubtractTimes(userTime, _prevSysUserTime);

        uint64 sysTotal = kernelDelta + userDelta;
        int64 kernerlTotal = ::max((int64)0, (int64)kernelDelta - (int64)idleDelta);

        double usage = 0.0f;
        if (sysTotal > 0)
            usage = ((kernerlTotal + userDelta) * 100.f) / sysTotal;

        _prevSysIdleTime = idleTime;
        _prevSysKernelTime = kernelTime;
        _prevSysUserTime = userTime;

        return usage;
    }

    double GetProcessCPUUsage()
    {
        FILETIME creationTime, exitTime, kernelTime, userTime;
        if (::GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime)) {
            uint64 procKernelDelta = SubtractTimes(kernelTime, _prevProcKernelTime);
            uint64 procUserDelta = SubtractTimes(userTime, _prevProcUserTime);
            ULONGLONG procTotal = procKernelDelta + procUserDelta;

            ULONGLONG sysKernelDelta = SubtractTimes(kernelTime, _prevSysKernelTime);
            ULONGLONG sysUserDelta = SubtractTimes(userTime, _prevSysUserTime);
            ULONGLONG sysTotal = sysKernelDelta + sysUserDelta;

            double usage = 0.0;
            if (sysTotal > 0)
                usage = (procTotal * 100.f / sysTotal);

            _prevProcKernelTime = kernelTime;
            _prevProcUserTime = userTime;

            return usage;
        }
        return 0.0;
    }

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

    FILETIME _prevSysIdleTime;
    FILETIME _prevSysKernelTime;
    FILETIME _prevSysUserTime;
    FILETIME _prevProcKernelTime;
    FILETIME _prevProcUserTime;
};


class MonitoringInfo
{
public:
	void SetService(shared_ptr<Service> service) { _service = service; }
	shared_ptr<Service> GetService() { return _service; }

	DWORDLONG	GetTotalVirtualMemory();
	DWORDLONG	GetVirtualMemoryCurrentlyUsed();
	SIZE_T		GetVirtualMemoryUsedByMe();
	DWORDLONG	GetTotalPhysicalMemory();
	DWORDLONG	GetPhysicalMemoryCurrentlyUsed();
	SIZE_T		GetPhysicalMemoryUsedByMe();
	double		GetCPUCurrentyUsed();
	double		GetCPUUsedByMe();

private:
	shared_ptr<Service> _service;
    CPUUsage _cpuUsage;
};
