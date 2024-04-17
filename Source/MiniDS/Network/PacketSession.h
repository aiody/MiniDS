// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class MINIDS_API PacketSession : public TSharedFromThis<PacketSession>
{
public:
	PacketSession(class FSocket* Socket);
	~PacketSession();

	void Run();

	void HandleRecvPackets();

	void Disconnect();

public:
	class FSocket* Socket;

	TSharedPtr<class RecvWorker> RecvWorkerThread;

	// GameThread와 NetworkThread가 데이터를 주고받는 공용 큐
	TQueue<TArray<uint8>> RecvPacketQueue;
};
