// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerPacketHandler.h"
#include "ClientSocket2.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handler_S_CHAT(Protocol::S_CHAT& pkt)
{
	FString msg(pkt.msg().c_str());
	UE_LOG(LogClass, Log, TEXT("Recv Payload = Msg : %s,  Size : %d"), *msg, msg.Len());

	Protocol::C_CHAT PktChat;
	PktChat.set_msg(pkt.msg());
	TSharedRef<FArrayWriter> SendBuffer = ServerPacketHandler::MakeSendBuffer(PktChat);

	ClientSocket2* Socket = ClientSocket2::Instance();
	Socket->Send(SendBuffer);

	return true;
}
