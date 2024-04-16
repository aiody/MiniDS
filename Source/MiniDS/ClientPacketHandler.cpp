// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientPacketHandler.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];
UMiniDSGameInstance* ServerPacketHandler::GameInstance = nullptr;

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

bool Handler_S_EnterGame(Protocol::S_EnterGame& pkt)
{
	UE_LOG(LogClass, Log, TEXT("Your Id is = %d"), pkt.playerid());

	return true;
} 

bool Handler_S_Spawn(Protocol::S_Spawn& pkt)
{
	UE_LOG(LogClass, Log, TEXT("Spawn Id is = %d"), pkt.player().id());

	if (ServerPacketHandler::GameInstance)
	{
		UE_LOG(LogClass, Log, TEXT("Spawn Weber = "), pkt.player().id());
		ServerPacketHandler::GameInstance->SpawnWeber();
	}
	
	return true;
}
