// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkWorker.h"
#include "Sockets.h"
#include "PacketSession.h"
#include "SendBuffer.h"

RecvWorker::RecvWorker(FSocket* Socket, TSharedPtr<PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorker Thread"));
}

RecvWorker::~RecvWorker()
{
}

bool RecvWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
	return true;
}

uint32 RecvWorker::Run()
{
	while (bRunning)
	{
		TArray<uint8> Packet;
		if (RecvPacket(OUT Packet))
		{
			if (TSharedPtr<PacketSession> Session = SessionRef.Pin())
			{
				Session->RecvPacketQueue.Enqueue(Packet);
			}
		}
	}

	return 0;
}

void RecvWorker::Exit()
{
}

void RecvWorker::Destroy()
{
	bRunning = false;
}

bool RecvWorker::RecvPacket(TArray<uint8>& OutPacket)
{
	// 패킷 헤더 파싱
	const int32 HeaderSize = sizeof(FPacketHeader);
	OutPacket.AddZeroed(HeaderSize);

	if (RecvDesiredBytes(OutPacket.GetData(), HeaderSize) == false)
		return false;

	// Size, Id 추출
	FPacketHeader Header;
	{
		FMemoryReader Reader(OutPacket);
		Reader << Header;
		//UE_LOG(LogTemp, Log, TEXT("Recv PacketId : %d, PacketSize : %d"), Header.PacketId, Header.PacketSize);
	}

	// 패킷 내용 파싱
	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	if (PayloadSize == 0)
		return true;

	OutPacket.AddZeroed(PayloadSize);

	if (RecvDesiredBytes(&OutPacket[HeaderSize], PayloadSize) == false)
		return false;

	return true;
}

bool RecvWorker::RecvDesiredBytes(uint8* Results, int32 Size)
{
	uint32 PendingDataSize;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;
	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}

SendWorker::SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorker Thread"));
}

SendWorker::~SendWorker()
{
}

bool SendWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));
	return true;
}

uint32 SendWorker::Run()
{
	while (bRunning)
	{
		SendBufferRef SendBuffer;
		if (TSharedPtr<PacketSession> Session = SessionRef.Pin())
		{
			if (Session->SendPacketQueue.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}
	}

	return 0;
}

void SendWorker::Exit()
{
}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker::Destroy()
{
	bRunning = false;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (Socket->Send(Buffer, Size, BytesSent) == false)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}
