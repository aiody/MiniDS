// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}

bool ClientSocket::InitSocket()
{
	// winsock 초기화
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	// 소켓 생성
	ServerSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ServerSocket == INVALID_SOCKET)
		return false;

	return true;
}

void ClientSocket::CloseSocket()
{
	::closesocket(ServerSocket);
	::WSACleanup();
}

bool ClientSocket::Connect(const char* ip, int port)
{
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = ::htons(port);
	::inet_pton(AF_INET, ip, &serverAddr.sin_addr);

	if (SOCKET_ERROR == ::connect(ServerSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
		return false;

	return true;
}

int ClientSocket::SendChatMessage(FString msg)
{
	char sendBuffer[4096];
	((PacketHeader*)sendBuffer)->size = sizeof(PacketHeader) + msg.Len();
	((PacketHeader*)sendBuffer)->id = 1;
	::memcpy(&sendBuffer[4], TCHAR_TO_ANSI(*msg), msg.Len());

	int resultCode = ::send(ServerSocket, const_cast<const char*>(sendBuffer), ((PacketHeader*)sendBuffer)->size, 0);
	if (resultCode == SOCKET_ERROR)
		return 0;
	UE_LOG(LogClass, Log, TEXT("SENT!! :: %d"), resultCode);
	return resultCode;
}

void ClientSocket::Dispatch()
{
	while (true)
	{
		char recvBuffer[1024] = {0};
		int recvLen = ::recv(ServerSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen > 0)
		{
			FString str(recvBuffer);
			UE_LOG(LogClass, Log, TEXT("RECEIVED!! :: %s"), *str);

			SendChatMessage("Yo ho~!");
		}
	}
}
