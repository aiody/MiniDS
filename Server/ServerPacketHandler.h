#pragma once
#include "Protocol.pb.h"

using PacketHandleFunc = std::function<bool(shared_ptr<PacketSession>&, BYTE*, int32)>;
extern PacketHandleFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_CHAT = 1001,
	PKT_S_CHAT = 1002,
	PKT_C_ENTER_GAME = 1003,
	PKT_S_ENTER_GAME = 1004,
	PKT_C_LEAVE_GAME = 1005,
	PKT_S_LEAVE_GAME = 1006,
	PKT_S_SPAWN = 1007,
	PKT_S_DESPAWN = 1008,
	PKT_C_MOVE = 1009,
	PKT_S_MOVE = 1010,
};

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len);
bool Handler_C_CHAT(shared_ptr<PacketSession>& session, Protocol::C_CHAT& pkt);
bool Handler_C_ENTER_GAME(shared_ptr<PacketSession>& session, Protocol::C_ENTER_GAME& pkt);
bool Handler_C_LEAVE_GAME(shared_ptr<PacketSession>& session, Protocol::C_LEAVE_GAME& pkt);
bool Handler_C_MOVE(shared_ptr<PacketSession>& session, Protocol::C_MOVE& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handler_INVALID;

		GPacketHandler[PKT_C_CHAT] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_CHAT>(Handler_C_CHAT, session, buffer, len); };
		GPacketHandler[PKT_C_ENTER_GAME] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTER_GAME>(Handler_C_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_C_LEAVE_GAME] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LEAVE_GAME>(Handler_C_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_C_MOVE] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MOVE>(Handler_C_MOVE, session, buffer, len); };
	}

	static bool HandlePacket(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_S_CHAT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_GAME); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_LEAVE_GAME); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_SPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_DESPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_DESPAWN); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_S_MOVE); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (false == pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)))
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static shared_ptr<SendBuffer> MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};
