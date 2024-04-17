// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketSession.h"
#include "NetworkWorker.h"
#include "Sockets.h"
#include "ClientPacketHandler.h"

PacketSession::PacketSession(FSocket* Socket) : Socket(Socket)
{
	ClientPacketHandler::Init();
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
}

void PacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		TSharedPtr<PacketSession> ThisRef = AsShared();
		ClientPacketHandler::HandlePacket(ThisRef, Packet.GetData(), Packet.Num());
	}
}

void PacketSession::Disconnect()
{
}
