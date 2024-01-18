#include "pch.h"
#include "SocketUtils.h"
#include "NetAddress.h"

LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;

void SocketUtils::Init()
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);

	// 런타임에 가져와야 해야하는 함수 포인터들
	SOCKET dummySocket = CreateSocket();
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	CloseSocket(dummySocket);
}

void SocketUtils::Clear()
{
	::WSACleanup();
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD dwBytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT &dwBytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

void SocketUtils::CloseSocket(SOCKET socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN sockAddr;
	::memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); // INADDR_ANY : 와일드카드 주소(0.0.0.0). 나의 네트워크 주소를 자동으로 할당
	sockAddr.sin_port = ::htons(port); // host byte 순서를 network byte 순서로 변환(little endian -> big endian). 그런데 ushort 타입을 쓰는.

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&sockAddr), sizeof(sockAddr));
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	// 소켓이 닫혔을 때 버퍼에 덜 보낸 데이터가 있다면 닫기를 지연한다.
	LINGER option;
	option.l_onoff = onoff; // 옵션의 on/off
	option.l_linger = linger; // 지연 시간
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool reuse)
{
	// 소켓을 이미 사용된 주소와 바인딩되도록 한다.
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, reuse);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	// recv buffer size를 지정한다.
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	// send buffer size를 지정한다.
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool noDelay)
{
	// TCP 소켓에 Nagle 알고리즘을 껐다 켰다 한다.
	// 기본 옵션은 FALSE 임. TRUE로 바꾸면 Nagle 알고리즘을 사용하지 않아서 딜레이를 없애줌.
	/* 
	Nagle 알고리즘 :
	ACK(ackowledgement, 응답수신)를 기다렸다가 도착하면 그동안 쌓인 데이터를 한번에 보냄.
	빈번한 전송을 줄이면서 효율적이지만 반응 시간이 떨어지는 단점이 있음.
	*/
	return SetSockOpt(socket, IPPROTO_TCP, TCP_NODELAY, noDelay);
}

bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	// AcceptEx 함수와 사용한다. listen 소켓의 문맥을 accept 소켓에도 적용한다.
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}
