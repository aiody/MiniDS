#include "pch.h"
#include "MonitoringPacketHandler.h"
#include "ThreadManager.h"
#include "Service.h"
#include "MonitoringService.h"

enum
{
    WORKER_TICK = 64
};

void DoWorkerJob(const shared_ptr<ClientService>& service)
{
    while (true)
    {
        LEndTickCount = ::GetTickCount64() + WORKER_TICK; // WORKER_TICK 만큼만 일할 것

        // 네트워크 입출력 처리 => JobQueue에 일감이 쌓임 => 쌓으면서 일부 일감 처리
        service->GetIocpCore()->Dispatch(10);

        // 예약된 일감(TImer) 중 시간이 다 된 일감이 있으면 JobQueue에 쌓음
        ThreadManager::DistributeReservedJobs();

        // 글로벌 큐에 쌓인 일감을 처리
        ThreadManager::DoGlobalQueueWork();
    }
}

int main()
{
    MonitoringPacketHandler::Init();

    shared_ptr<ClientService> service = make_shared<ClientService>(
        NetAddress(L"127.0.0.1", 9999),
        make_shared<IocpCore>(),
        [=]() ->shared_ptr<MonitoringSession> { return make_shared<MonitoringSession>(); },
        1);

    ASSERT_CRASH(service->Start());

    gThreadManager->Launch([=]()
    {
        DoWorkerJob(service);
    });

    gJobQueue->Push(make_shared<Job>(gMonitoringService, &MonitoringService::Start));

    DoWorkerJob(service);

    gThreadManager->Join();

    service->CloseService();
}
