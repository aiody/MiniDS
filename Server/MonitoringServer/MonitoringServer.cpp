#include "pch.h"
#include "MonitoringPacketHandler.h"
#include "ThreadManager.h"
#include "Service.h"
#include "MonitoringService.h"

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
        while (true)
        {
            service->GetIocpCore()->Dispatch(10);
            gJobTimer->Distribute(::GetTickCount64());
        }
    });

    gJobQueue->Push(make_shared<Job>(gMonitoringService, &MonitoringService::Start));

    while (true)
        gJobQueue->Flush();

    gThreadManager->Join();

    service->CloseService();
}
