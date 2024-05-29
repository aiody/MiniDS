#include "pch.h"
#include "NetAddress.h"
#include "ThreadManager.h"
#include "Service.h"
#include "GameSession.h"
#include "JobQueue.h"
#include "JobTimer.h"
#include "Room.h"

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
                    service->GetIocpCore()->Dispatch(10);

                    gJobTimer->Distribute(::GetTickCount64());
                }
            });
    }

    gJobQueue->Push(make_shared<Job>(gRoom, &Room::Start));

    while (true)
        gJobQueue->Flush();
    
    gThreadManager->Join();

    service->CloseService();
}
