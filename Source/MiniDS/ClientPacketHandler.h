// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniDS.h"
#include "Protocol/Protocol.pb.h"
#include "SendBuffer.h"

using PacketHandleFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandleFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_ENTER_GAME = 1001,
	PKT_S_ENTER_GAME = 1002,
	PKT_C_LEAVE_GAME = 1003,
	PKT_S_LEAVE_GAME = 1004,
	PKT_S_SPAWN = 1005,
	PKT_S_DESPAWN = 1006,
	PKT_C_MOVE = 1007,
	PKT_S_MOVE = 1008,
	PKT_C_ATTACK = 1009,
	PKT_S_HIT = 1010,
	PKT_S_DEATH = 1011,
};

bool Handler_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handler_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handler_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt);
bool Handler_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handler_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt);
bool Handler_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt);
bool Handler_S_HIT(PacketSessionRef& session, Protocol::S_HIT& pkt);
bool Handler_S_DEATH(PacketSessionRef& session, Protocol::S_DEATH& pkt);

/**
 *
 */
class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handler_INVALID;

		GPacketHandler[PKT_S_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handler_S_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_LEAVE_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handler_S_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handler_S_SPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handler_S_DESPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handler_S_MOVE, session, buffer, len); };
		GPacketHandler[PKT_S_HIT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_HIT>(Handler_S_HIT, session, buffer, len); };
		GPacketHandler[PKT_S_DEATH] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DEATH>(Handler_S_DEATH, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		FPacketHeader* header = reinterpret_cast<FPacketHeader*>(buffer);
		return GPacketHandler[header->PacketId](session, buffer, len);
	}

	static SendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ATTACK& pkt) { return MakeSendBuffer(pkt, PKT_C_ATTACK); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (false == pkt.ParseFromArray(buffer + sizeof(FPacketHeader), len - sizeof(FPacketHeader)))
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const int dataSize = pkt.ByteSize();
		const int packetSize = sizeof(FPacketHeader) + dataSize;

		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);

		FPacketHeader* header = reinterpret_cast<FPacketHeader*>(sendBuffer->Buffer());
		header->PacketSize = packetSize;
		header->PacketId = pktId;

		pkt.SerializeToArray(&header[1], dataSize);

		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};
