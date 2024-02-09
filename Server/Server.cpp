#include "pch.h"
#include "NetAddress.h"
#include "ThreadManager.h"
#include "Session.h"
#include "Service.h"

class GameSession : public PacketSession
{
public:
    virtual void OnPacketRecv(BYTE* buffer, int32 len) override
    {
        PacketHeader header = *(reinterpret_cast<PacketHeader*>(buffer));
        cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

        char recvBuffer[4096] = {0};
        ::memcpy(recvBuffer, &buffer[4], header.size - sizeof(PacketHeader));
        cout << "OnPacketRecv Message = " << recvBuffer << ", Len = " << header.size << endl;

        shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
        sendBuffer->CopyData(recvBuffer, header.size - sizeof(PacketHeader));

        for (int i = 0; i < 5; i++)
            Send(sendBuffer);

        this_thread::sleep_for(1s);
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
