// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientPacketHandler.h"
#include "PacketSession.h"
#include "MiniDSGameInstance.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	FPacketHeader* header = reinterpret_cast<FPacketHeader*>(buffer);
	return false;
}

bool Handler_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	FString msg(pkt.msg().c_str());
	UE_LOG(LogClass, Log, TEXT("Recv Payload = Msg : %s,  Size : %d"), *msg, msg.Len());

	Protocol::C_CHAT PktChat;
	PktChat.set_msg(pkt.msg());
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(PktChat);
	Cast<UMiniDSGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);

	return true;
}

bool Handler_S_EnterGame(PacketSessionRef& session, Protocol::S_EnterGame& pkt)
{
	UE_LOG(LogClass, Log, TEXT("Your Id is = %d"), pkt.playerid());

	return true;
}

bool Handler_S_Spawn(PacketSessionRef& session, Protocol::S_Spawn& pkt)
{
	UE_LOG(LogClass, Log, TEXT("Spawn Id is = %d"), pkt.player().id());

	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->SpawnWeber();
	}

	return true;
}
