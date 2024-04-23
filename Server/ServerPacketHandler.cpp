#include "pch.h"
#include "ServerPacketHandler.h"
#include "Room.h"
#include "Player.h"
#include "ObjectUtils.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handler_C_CHAT(shared_ptr<PacketSession>& session, Protocol::C_CHAT& pkt)
{
	shared_ptr<Job> job = make_shared<Job>([session, pkt]() {
		cout << "ChatJob = " << pkt.msg() << endl;

		this_thread::sleep_for(1s);

		Protocol::S_CHAT pkt_chat;
		pkt_chat.set_msg(pkt.msg());
		shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt_chat);
		session->Send(sendBuffer);
		});

	//gJobQueue->Push(job);
	gJobTimer->Reserve(3000, job);

	return true;
}

bool Handler_C_ENTER_GAME(shared_ptr<PacketSession>& session, Protocol::C_ENTER_GAME& pkt)
{
	shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

	// 플레이어 생성
	shared_ptr<Player> player = ObjectUtils::CreatePlayer(gameSession);
	
	// 방에 입장
	gJobQueue->Push(make_shared<Job>(gRoom, &Room::HandleEnterPlayer, player));

	return true;
}
