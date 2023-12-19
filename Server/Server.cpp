#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    // wVersionRequested : 버전 정보
    // lpWSAData : 윈도우 소켓의 디테일한 정보를 반환하는 LPWSADATA 구조체
    // return : 성공하면 0을 리턴하고 아니면 에러코드를 WSAGetLastError를 통해 얻을 수 있다.
    WSADATA wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        int errorCode = ::WSAGetLastError();
        return 0;
    }

    // af : address family
    // type : socket type
    // protocol : protocol, af와 type이 결정되면 결정됨
    // return : descriptor including socket information
    SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        int errorCode = ::WSAGetLastError();
    }


    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(9999);
    ::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // s : socket
    // name : sockaddr 구조체의 포인터
    // namelen : name의 길이
    if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0)
    {
        int errorCode = ::WSAGetLastError();
    }


    if (listen(listenSocket, 10) != 0)
    {
        int errorCode = ::WSAGetLastError();
    }

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

    WSACleanup();
}
