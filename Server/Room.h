#pragma once
#include "Player.h"

class Room : public enable_shared_from_this<Room>
{
public:
	void Enter(shared_ptr<Player> player);
	void Leave(shared_ptr<Player> player);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer);

private:
	map<uint64, shared_ptr<Player>> _players;
};

extern shared_ptr<Room> gRoom;
