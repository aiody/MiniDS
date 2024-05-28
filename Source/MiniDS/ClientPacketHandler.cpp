// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientPacketHandler.h"
#include "PacketSession.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	FPacketHeader* header = reinterpret_cast<FPacketHeader*>(buffer);
	return false;
}

bool Handler_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handler_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLeaveGame();
	}

	return true;
}

bool Handler_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handler_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);
	}

	return true;
}

bool Handler_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleMove(pkt);
	}

	return true;
}

bool Handler_S_HIT(PacketSessionRef& session, Protocol::S_HIT& pkt)
{
	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleHit(pkt);
	}

	return true;
}

bool Handler_S_DEATH(PacketSessionRef& session, Protocol::S_DEATH& pkt)
{
	if (auto* GameInstance = Cast<UMiniDSGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDeath(pkt);
	}

	return true;
}
