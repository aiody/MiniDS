// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol/Protocol.pb.h"
#include "Serialization/ArrayWriter.h"
#include "NetworkWorker.h"
#include "PacketSession.h"

class PacketSession;

using PacketHandleFunc = std::function<bool(TSharedPtr<PacketSession>&, BYTE*, int32)>;
extern PacketHandleFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_CHAT = 1001,
	PKT_S_CHAT = 1002,
	PKT_S_EnterGame = 1003,
	PKT_S_Spawn = 1004,
};

bool Handler_INVALID(TSharedPtr<PacketSession>& session, BYTE* buffer, int32 len);
bool Handler_S_CHAT(TSharedPtr<PacketSession>& session, Protocol::S_CHAT& pkt);
bool Handler_S_EnterGame(TSharedPtr<PacketSession>& session, Protocol::S_EnterGame& pkt);
bool Handler_S_Spawn(TSharedPtr<PacketSession>& session, Protocol::S_Spawn& pkt);

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

		GPacketHandler[PKT_S_CHAT] = [](TSharedPtr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handler_S_CHAT, session, buffer, len); };
		GPacketHandler[PKT_S_EnterGame] = [](TSharedPtr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_EnterGame>(Handler_S_EnterGame, session, buffer, len); };
		GPacketHandler[PKT_S_Spawn] = [](TSharedPtr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_Spawn>(Handler_S_Spawn, session, buffer, len); };
	}

	static bool HandlePacket(TSharedPtr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		FPacketHeader* header = reinterpret_cast<FPacketHeader*>(buffer);
		return GPacketHandler[header->PacketId](session, buffer, len);
	}

	static TSharedRef<FArrayWriter> MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, TSharedPtr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (false == pkt.ParseFromArray(buffer + sizeof(FPacketHeader), len - sizeof(FPacketHeader)))
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static TSharedRef<FArrayWriter> MakeSendBuffer(T& pkt, uint16 pktId)
	{
		TSharedRef<FArrayWriter> Writer = MakeShared<FArrayWriter>();
		const int DataSize = pkt.ByteSize();
		const int PacketSize = sizeof(FPacketHeader) + DataSize;
		Writer->SetNum(PacketSize);
		FPacketHeader Header(PacketSize, pktId);
		*Writer << Header;
		pkt.SerializeToArray(Writer->GetData() + sizeof(FPacketHeader), DataSize);

		return Writer;
	}
};
