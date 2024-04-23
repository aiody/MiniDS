#pragma once

class Player;

class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	USE_LOCK;

	bool HandleEnterPlayer(shared_ptr<Player> player);

private:
	bool EnterPlayer(shared_ptr<Player> player);
	void LeavePlayer(shared_ptr<Player> player);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, shared_ptr<Player>> _players;
};

extern shared_ptr<Room> gRoom;
