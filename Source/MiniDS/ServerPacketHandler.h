// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientSocket2.h"
#include "Protocol/Protocol.pb.h"
#include "Serialization/ArrayWriter.h"
#include <functional>

using PacketHandleFunc = std::function<bool(BYTE*, int32)>;
extern PacketHandleFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_CHAT = 1001,
	PKT_S_CHAT = 1002,
};

bool Handler_INVALID(BYTE* buffer, int32 len);
bool Handler_S_CHAT(Protocol::S_CHAT& pkt);

/**
 * 
 */
class MINIDS_API ServerPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handler_INVALID;

		GPacketHandler[PKT_S_CHAT] = [](BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handler_S_CHAT, buffer, len); };
	}

	static bool HandlePacket(BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](buffer, len);
	}

	static TSharedRef<FArrayWriter> MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (false == pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)))
			return false;

		return func(pkt);
	}

	template<typename T>
	static TSharedRef<FArrayWriter> MakeSendBuffer(T& pkt, uint16 pktId)
	{
		TSharedRef<FArrayWriter> Writer = MakeShared<FArrayWriter>();
		const int DataSize = pkt.ByteSize();
		const int PacketSize = sizeof(PacketHeader) + DataSize;
		Writer->SetNum(PacketSize);
		PacketHeader Header(PacketSize, pktId);
		*Writer << Header;
		pkt.SerializeToArray(Writer->GetData() + sizeof(PacketHeader), DataSize);

		return Writer;
	}
};
