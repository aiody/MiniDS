#pragma once

class MonitoringService : public enable_shared_from_this<MonitoringService>
{
public:
	void Start();

	void SetLastRespondTime() { _getLastRespondTime = ::GetTickCount64(); }
	void SetMonitoringInfo(uint64 cpuUsage, uint64 memoryUsage, uint64 numOfSession, uint64 numOfThread, string serverStartTime, uint64 packetTPS);
	void SetServerRunningStatus(bool running) { _isServerRunning = running; }

private:
	void UpdateTick();
	void DrawUI();
	void HealthCheck();

private:
	uint64 _getLastRespondTime = 0;
	bool _isServerRunning = false;
	uint64 _cpuUsage = 0;
	uint64 _memoryUsage = 0;
	uint64 _numOfSession = 0;
	uint64 _numOfThread = 0;
	string _serverStartTime = "";
	uint64 _packetTPS = 0;
};

extern shared_ptr<MonitoringService> gMonitoringService;
