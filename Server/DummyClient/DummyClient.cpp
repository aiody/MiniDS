#include "pch.h"
#include "DummyClientPacketHandler.h"
#include "Service.h"
#include "ThreadManager.h"
#include "DummyClientSession.h"

int main()
{
    DummyClientPacketHandler::Init();

    shared_ptr<ClientService> service = make_shared<ClientService>(
        NetAddress(L"127.0.0.1", 9999),
        make_shared<IocpCore>(),
        [=]() ->shared_ptr<DummyClientSession> { return make_shared<DummyClientSession>(); },
        500);

    ASSERT_CRASH(service->Start());

    gThreadManager->Launch([=]()
        {
            while (true)
            {
                service->GetIocpCore()->Dispatch(10);
                gJobTimer->Distribute(::GetTickCount64());
            }
        });

    while (true)
        gJobQueue->Flush();

    gThreadManager->Join();

    service->CloseService();
}
