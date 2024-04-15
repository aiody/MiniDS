#include "pch.h"
#include "Room.h"
#include "GameSession.h"

shared_ptr<Room> gRoom = make_shared<Room>();

void Room::Enter(shared_ptr<Player> player)
{
	_players[player->playerId] = player;
}

void Room::Leave(shared_ptr<Player> player)
{
	_players.erase(player->playerId);
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto& p : _players)
	{
		p.second->ownerSession->Send(sendBuffer);
	}
}
