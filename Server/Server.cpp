#include "pch.h"
#include "SocketUtils.h"
#include "NetAddress.h"
#include "Listener.h"

int main()
{
    shared_ptr<Listener> listener = make_shared<Listener>();
    listener->StartAccept(NetAddress(L"127.0.0.1", 9999));

    while (true)
    {
        GIocpCore.Dispatch();
    }

    listener->CloseSocket();
}
