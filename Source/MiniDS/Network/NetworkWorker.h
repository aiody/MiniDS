// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MiniDS.h"

class FSocket;

struct MINIDS_API FPacketHeader
{
	FPacketHeader() : PacketSize(0), PacketId(0) { }
	FPacketHeader(uint16 PacketSize, uint16 PacketId) : PacketSize(PacketSize), PacketId(PacketId) { }

	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.PacketId;
		return Ar;
	}

	uint16 PacketSize;
	uint16 PacketId;
};

/**
 * 
 */
class MINIDS_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~RecvWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool RecvPacket(TArray<uint8>& OutPacket);
	bool RecvDesiredBytes(uint8* Results, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket = nullptr;
	TWeakPtr<class PacketSession> SessionRef;
};

/**
 *
 */
class MINIDS_API SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(SendBufferRef SendBuffer);

	void Destroy();

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket = nullptr;
	TWeakPtr<class PacketSession> SessionRef;
};
