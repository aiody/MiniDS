#include "pch.h"
#include "SocketUtils.h"

int main()
{
    SOCKET listenSocket = SocketUtils::CreateSocket();
    if (listenSocket == INVALID_SOCKET)
    {
        int errorCode = ::WSAGetLastError();
    }

    SocketUtils::BindAnyAddress(listenSocket, 9999);

    SocketUtils::Listen(listenSocket);

    cout << "Listening..." << endl;

    while (true)
    {
        // s : listen socket
        // addr : 연결된 주소가 채워져서 나옴
        // addrlen : addr의 빈공간의 크기를 넣으면 리턴하고나서는 addr의 크기가 나옴
        SOCKADDR_IN clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET)
        {
            int errorCode = ::WSAGetLastError();
            break;
        }

        cout << "Connected!!!" << endl;

        while (true)
        {
            // s : connected socket
            // buf : pointer of receive buffer
            // len : buffer의 크기
            // flags : 옵션을 정의할 때 사용
            BYTE recvBuffer[1024];
            ::memset(recvBuffer, 0, sizeof(recvBuffer));
            int recvLen = ::recv(clientSocket, (char*)recvBuffer, sizeof(recvBuffer), NULL);

            if (recvLen <= 0)
            {
                int errorCode = ::WSAGetLastError();
                break;
            }

            cout << recvBuffer << endl;

            // s : connected socket
            // buf : pointer of send buffer
            // len : buffer의 크기
            // flags : 옵션을 정의할 때 사용
            int numOfBytes = ::send(clientSocket, (char*)recvBuffer, recvLen, NULL);
            if (numOfBytes == SOCKET_ERROR)
            {
                int errorCode = ::WSAGetLastError();
                break;
            }

            Sleep(1000);
        }
    }

    closesocket(listenSocket);
}
