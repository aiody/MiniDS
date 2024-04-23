// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#define USING_SHARED_PTR(name)		using name##Ref = TSharedPtr<class name>;

USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);

#include "MiniDSGameInstance.h"
#include "ClientPacketHandler.h"
#include "Engine/World.h"
#define SEND_PACKET(Pkt)															\
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);			\
	Cast<UMiniDSGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
