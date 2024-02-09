// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include "CoreMinimal.h"

/**
 * 
 */
class MINIDS_API ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	static ClientSocket* Instance()
	{
		static ClientSocket instance;
		return &instance;
	}

	bool InitSocket();
	void CloseSocket();
	bool Connect(const char* ip, int port);

	int SendChatMessage(FString msg);

	void Dispatch();

private:
	SOCKET ServerSocket;
};

struct PacketHeader
{
	int16 size;
	int16 id;
};
