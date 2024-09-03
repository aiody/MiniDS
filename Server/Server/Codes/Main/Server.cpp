#include "pch.h"
#include "NetAddress.h"
#include "ThreadManager.h"
#include "Service.h"
#include "GameSession.h"
#include "JobQueue.h"
#include "JobTimer.h"
#include "Room.h"
#include "ServerPacketHandler.h"
#include "MonitoringInfo.h"

enum
{
    WORKER_TICK = 64
};

void DoWorkerJob(const shared_ptr<ServerService>& service)
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
    ServerPacketHandler::Init();
    
    shared_ptr<ServerService> service = make_shared<ServerService>(
        NetAddress(L"127.0.0.1", 9999),
        make_shared<IocpCore>(),
        [=]() ->shared_ptr<GameSession> { return make_shared<GameSession>(); },
        100);

    ASSERT_CRASH(service->Start());

    gMonitoringInfo->SetService(service);
    gRoom->Push(make_shared<Job>(gRoom, &Room::Start));

    uint32 threadCnt = ThreadManager::GetThreadCountToLaunch();
    for (int32 i = 0; i < threadCnt; i++)
    {
        gThreadManager->Launch([&service]()
            {
                DoWorkerJob(service);
            });
    }

    DoWorkerJob(service);
    
    gThreadManager->Join();

    service->CloseService();
}
