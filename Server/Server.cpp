#include "pch.h"
#include "NetAddress.h"
#include "ThreadManager.h"
#include "Service.h"
#include "GameSession.h"

int main()
{
    shared_ptr<ServerService> service = make_shared<ServerService>(
        NetAddress(L"127.0.0.1", 9999),
        make_shared<IocpCore>(),
        [=]() ->shared_ptr<GameSession> { return make_shared<GameSession>(); },
        100);

    ASSERT_CRASH(service->Start());

    for (int32 i = 0; i < 5; i++)
    {
        gThreadManager->Launch([=]()
            {
                while (true)
                {
                    service->GetIocpCore()->Dispatch();
                }
            });
    }
    
    gThreadManager->Join();

    service->CloseService();
}
