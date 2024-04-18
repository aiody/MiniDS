// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MiniDS.h"
#include "Protocol/Protocol.pb.h"
#include "Serialization/ArrayWriter.h"
#include "NetworkWorker.h"
#include "SendBuffer.h"

using PacketHandleFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandleFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_CHAT = 1001,
	PKT_S_CHAT = 1002,
	PKT_S_EnterGame = 1003,
	PKT_S_Spawn = 1004,
};

bool Handler_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handler_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt);
bool Handler_S_EnterGame(PacketSessionRef& session, Protocol::S_EnterGame& pkt);
bool Handler_S_Spawn(PacketSessionRef& session, Protocol::S_Spawn& pkt);

/**
 *
 */
class MINIDS_API ClientPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handler_INVALID;

		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handler_S_CHAT, session, buffer, len); };
		GPacketHandler[PKT_S_EnterGame] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_EnterGame>(Handler_S_EnterGame, session, buffer, len); };
		GPacketHandler[PKT_S_Spawn] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_Spawn>(Handler_S_Spawn, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		FPacketHeader* header = reinterpret_cast<FPacketHeader*>(buffer);
		return GPacketHandler[header->PacketId](session, buffer, len);
	}

	static SendBufferRef MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }

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
