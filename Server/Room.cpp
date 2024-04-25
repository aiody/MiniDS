#include "pch.h"
#include "Room.h"
#include "Player.h"

shared_ptr<Room> gRoom = make_shared<Room>();

Room::Room()
{
}

Room::~Room()
{
}

bool Room::HandleEnterPlayer(shared_ptr<Player> player)
{
	WRITE_LOCK;

	bool success = EnterPlayer(player);

	player->playerInfo->set_x(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_y(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_z(118.f);

	// 해당 플레이어에게 게임에 접속함을 알림
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->playerInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		if (auto session = player->session.lock())
		{
			SEND_PACKET(enterGamePkt);
		}
	}

	// 현재 플레이어에게 다른 플레이어가 있음을 알림
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _players)
		{
			shared_ptr<Player> p = item.second;
			if (p->playerInfo->id() == player->playerInfo->id())
				continue;

			Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*p->playerInfo);
		}

		if (auto session = player->session.lock())
		{
			SEND_PACKET(spawnPkt);
		}
	}

	// 다른 플레이어에게 현재 플레이어가 게임에 접속함을 알림
	{
		Protocol::S_SPAWN spawnPkt;
		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom(*player->playerInfo);

		shared_ptr<SendBuffer> spawnBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		gJobQueue->Push(make_shared<Job>(gRoom, &Room::Broadcast, spawnBuffer, player->playerInfo->id()));
	}

	return success;
}

bool Room::HandleLeavePlayer(shared_ptr<Player> player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 id = player->playerInfo->id();
	bool success = LeavePlayer(id);

	// 해당 플레이어에게 게임에 퇴장함을 알림
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;
		if (auto session = player->session.lock())
		{
			SEND_PACKET(leaveGamePkt);
		}
	}

	// 다른 플레이어에게 현재 플레이어가 게임에서 퇴장한 사실을 알림
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_ids(id);

		shared_ptr<SendBuffer> despawnBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		gJobQueue->Push(make_shared<Job>(gRoom, &Room::Broadcast, despawnBuffer, id));

		// LeavePlayer에서 room에서 빼버렸기 때문에 Broadcast 대상에 들어가지 않아 다시 보냄
		if (auto session = player->session.lock())
			session->Send(despawnBuffer);
	}

	return true;
}

void Room::HandleMove(shared_ptr<Protocol::PlayerInfo> info)
{
	WRITE_LOCK;

	const uint64 id = info->id();
	if (_players.find(id) == _players.end())
		return;

	shared_ptr<Player>& player = _players[id];
	player->playerInfo->CopyFrom(*info);

	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PlayerInfo* moveInfo = movePkt.mutable_info();
			moveInfo->CopyFrom(*info);
		}
		shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}
}

bool Room::EnterPlayer(shared_ptr<Player> player)
{
	// 중복됨
	if (_players.find(player->playerInfo->id()) != _players.end())
		return false;

	_players.insert(::make_pair(player->playerInfo->id(), player));

	player->room.store(shared_from_this());

	return true;
}

bool Room::LeavePlayer(uint64 id)
{
	// 존재하는지 체크
	if (_players.find(id) == _players.end())
		return false;
	
	shared_ptr<Player> player = _players[id];
	player->room.store(weak_ptr<Room>());

	_players.erase(id);

	return true;
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBuffer, uint64 exceptId)
{
	for (auto& item : _players)
	{
		shared_ptr<Player> player = item.second;
		if (player->playerInfo->id() == exceptId)
			continue;
		if (shared_ptr<GameSession> session = player->session.lock())
			session->Send(sendBuffer);
	}
}
