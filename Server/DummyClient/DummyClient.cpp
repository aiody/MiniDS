#include "pch.h"
#include "DummyClientPacketHandler.h"
#include "Service.h"
#include "ThreadManager.h"
#include "DummyClientSession.h"

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
    DummyClientPacketHandler::Init();

    shared_ptr<ClientService> service = make_shared<ClientService>(
        NetAddress(L"127.0.0.1", 9999),
        make_shared<IocpCore>(),
        [=]() ->shared_ptr<DummyClientSession> { return make_shared<DummyClientSession>(); },
        50);

    ASSERT_CRASH(service->Start());

    gThreadManager->Launch([&service]()
        {
            DoWorkerJob(service);
        });

    DoWorkerJob(service);

    gThreadManager->Join();

    service->CloseService();
}
