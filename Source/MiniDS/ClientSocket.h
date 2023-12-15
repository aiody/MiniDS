// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>

#define BOOL bool

class MINIDS_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	static ClientSocket* Instance()
	{
		static ClientSocket instance;
		return &instance;
	}

	BOOL InitSocket();
	void CloseSocket();
	bool Connect(const char* ip, int port);

	// FRunnable override
	bool Init() override;
	uint32 Run() override;
	void Stop() override;
	void Exit() override;

private:
	SOCKET ServerSocket;
};
