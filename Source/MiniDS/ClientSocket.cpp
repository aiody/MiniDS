// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

// Sets default values
ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}

bool ClientSocket::InitSocket()
{
	// winsock √ ±‚»≠
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	ServerSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ServerSocket == INVALID_SOCKET)
		return false;

	return true;
}

void ClientSocket::CloseSocket()
{
}

bool ClientSocket::Connect(const char* ip, int port)
{
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = ::htons(port);
	::inet_pton(AF_INET, ip, &serverAddr.sin_addr);

	if (connect(ServerSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return false;

	return true;
}

bool ClientSocket::Init()
{
	return true;
}

uint32 ClientSocket::Run()
{
	return 0;
}

void ClientSocket::Stop()
{
}

void ClientSocket::Exit()
{
}
