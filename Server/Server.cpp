#include "pch.h"
#include "SocketUtils.h"
#include "NetAddress.h"
#include "Listener.h"
#include "ThreadManager.h"

int main()
{
    shared_ptr<Listener> listener = make_shared<Listener>();
    listener->StartAccept(NetAddress(L"127.0.0.1", 9999));

    gThreadManager->Launch([]()
        {
            while (true)
            {
                GIocpCore.Dispatch();
            }
        });
    
    gThreadManager->Join();

    listener->CloseSocket();
}
