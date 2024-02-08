#include "pch.h"
#include "NetAddress.h"
#include "ThreadManager.h"
#include "Session.h"
#include "Service.h"

class GameSession : public Session
{
public:
    virtual int32 OnRecv(BYTE* buffer, int32 len) override
    {
        // Echo
        cout << "OnRecv Len = " << len << endl;
        shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
        sendBuffer->CopyData(buffer, len);

        for (int i = 0; i < 5; i++)
            Send(sendBuffer);

        this_thread::sleep_for(1s);
        return len;
    }

    virtual void OnSend(int32 len) override
    {
        cout << "OnSend Len = " << len << endl;
    }
};

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
