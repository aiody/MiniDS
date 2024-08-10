#pragma once
#include "Protocol.pb.h"

using PacketHandleFunc = std::function<bool(shared_ptr<PacketSession>&, BYTE*, int32)>;
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
	PKT_M_REQ_SERVER_INFO = 2001,
	PKT_S_RES_SERVER_INFO = 2002,
};

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len);
bool Handler_S_ENTER_GAME(shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& pkt);
bool Handler_S_LEAVE_GAME(shared_ptr<PacketSession>& session, Protocol::S_LEAVE_GAME& pkt);
bool Handler_S_SPAWN(shared_ptr<PacketSession>& session, Protocol::S_SPAWN& pkt);
bool Handler_S_DESPAWN(shared_ptr<PacketSession>& session, Protocol::S_DESPAWN& pkt);
bool Handler_S_MOVE(shared_ptr<PacketSession>& session, Protocol::S_MOVE& pkt);
bool Handler_S_HIT(shared_ptr<PacketSession>& session, Protocol::S_HIT& pkt);
bool Handler_S_DEATH(shared_ptr<PacketSession>& session, Protocol::S_DEATH& pkt);

class DummyClientPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handler_INVALID;

		GPacketHandler[PKT_S_ENTER_GAME] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handler_S_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_LEAVE_GAME] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handler_S_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWN] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handler_S_SPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_DESPAWN] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handler_S_DESPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_MOVE] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handler_S_MOVE, session, buffer, len); };
		GPacketHandler[PKT_S_HIT] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_HIT>(Handler_S_HIT, session, buffer, len); };
		GPacketHandler[PKT_S_DEATH] = [](shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DEATH>(Handler_S_DEATH, session, buffer, len); };
	}

	static bool HandlePacket(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_ATTACK& pkt) { return MakeSendBuffer(pkt, PKT_C_ATTACK); }

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
