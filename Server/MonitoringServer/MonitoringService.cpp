#include "pch.h"
#include "MonitoringService.h"
#include "Job.h"
#include "ConsoleHelper.h"

shared_ptr<MonitoringService> gMonitoringService = make_shared<MonitoringService>();

void MonitoringService::Start()
{
	ConsoleHelper::Clear();

	_getLastRespondTime = ::GetTickCount64();

	UpdateTick();
}

void MonitoringService::SetMonitoringInfo(uint64 cpuUsage, uint64 memoryUsage, uint64 numOfSession, uint64 numOfThread, string serverStartTime, uint64 packetTPS)
{
	_cpuUsage = cpuUsage;
	_memoryUsage = memoryUsage;
	_numOfSession = numOfSession;
	_numOfThread = numOfThread;
	_serverStartTime = serverStartTime;
	_packetTPS = packetTPS;
}

void MonitoringService::UpdateTick()
{
	HealthCheck();
	DrawUI();
	gJobTimer->Reserve(1000, make_shared<Job>(shared_from_this(), &MonitoringService::UpdateTick));
}

void MonitoringService::DrawUI()
{
	ConsoleHelper::Clear();

	ConsoleHelper::SetCursorPosition(0, 0);
	cout << "Server Started : " << _serverStartTime;

	ConsoleHelper::SetCursorPosition(0, 1);
	cout << "Server State";

	ConsoleHelper::SetCursorPosition(20, 1);
	if (_isServerRunning)
	{
		ConsoleHelper::SetCursorColor(ConsoleColor::GREEN);
		cout << "Running";
	}
	else
	{
		ConsoleHelper::SetCursorColor(ConsoleColor::RED);
		cout << "Stopped" << endl;
	}
	ConsoleHelper::SetCursorColor(ConsoleColor::WHITE);

	ConsoleHelper::SetCursorPosition(0, 2);
	cout << "CPU Usage";
	ConsoleHelper::SetCursorPosition(20, 2);
	cout << fixed;
	cout.precision(3);
	cout << _cpuUsage << " %";
	cout.unsetf(ios::fixed);

	ConsoleHelper::SetCursorPosition(0, 3);
	cout << "Memory Used";
	ConsoleHelper::SetCursorPosition(20, 3);
	cout << _memoryUsage << " MB";

	ConsoleHelper::SetCursorPosition(40, 1);
	cout << "Num of Session";
	ConsoleHelper::SetCursorPosition(60, 1);
	cout << _numOfSession;

	ConsoleHelper::SetCursorPosition(40, 2);
	cout << "Num of Thread";
	ConsoleHelper::SetCursorPosition(60, 2);
	cout << _numOfThread;

	ConsoleHelper::SetCursorPosition(40, 3);
	cout << "Packet TPS";
	ConsoleHelper::SetCursorPosition(60, 3);
	cout << _packetTPS;

	ConsoleHelper::SetCursorPosition(0, 20);
	ConsoleHelper::ShowConsoleCursor(false);
}

void MonitoringService::HealthCheck()
{
	if (::GetTickCount64() - _getLastRespondTime > 5000)
		_isServerRunning = false;
}
