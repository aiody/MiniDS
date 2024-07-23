#include "pch.h"
#include "ServerPacketHandler.h"
#include "Room.h"
#include "Player.h"
#include "ObjectUtils.h"
#include "ThreadManager.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
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

bool Handler_C_LEAVE_GAME(shared_ptr<PacketSession>& session, Protocol::C_LEAVE_GAME& pkt)
{
	shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

	shared_ptr<Player> player = gameSession->curPlayer.load();
	if (player == nullptr)
		return false;

	shared_ptr<Room> room = player->room.load().lock();
	if (room == nullptr)
		return false;

	gJobQueue->Push(make_shared<Job>(room, &Room::HandleLeavePlayer, player));

	return true;
}

bool Handler_C_MOVE(shared_ptr<PacketSession>& session, Protocol::C_MOVE& pkt)
{
	shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

	shared_ptr<Player> player = gameSession->curPlayer.load();
	if (player == nullptr)
		return false;

	shared_ptr<Room> room = player->room.load().lock();
	if (room == nullptr)
		return false;
	
	gJobQueue->Push(make_shared<Job>(room, &Room::HandleMove, pkt));

	return true;
}

bool Handler_C_ATTACK(shared_ptr<PacketSession>& session, Protocol::C_ATTACK& pkt)
{
	shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

	shared_ptr<Player> player = gameSession->curPlayer.load();
	if (player == nullptr)
		return false;

	shared_ptr<Room> room = player->room.load().lock();
	if (room == nullptr)
		return false;

	gJobQueue->Push(make_shared<Job>(room, &Room::HandleAttack, pkt.from(), pkt.to()));

	return true;
}

bool Handler_M_REQ_SERVER_INFO(shared_ptr<PacketSession>& session, Protocol::M_REQ_SERVER_INFO& pkt)
{
	shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

	Protocol::S_RES_SERVER_INFO resServerInfoPkt;
	{
		resServerInfoPkt.set_game_session_num(33);
	}
	shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(resServerInfoPkt);
	gameSession->Send(sendBuffer);

	return false;
}
