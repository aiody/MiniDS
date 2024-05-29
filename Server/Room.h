#pragma once

class Player;
class Object;

class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	void Start();
	void UpdateTick();

	bool HandleEnterPlayer(shared_ptr<Player> player);
	bool HandleLeavePlayer(shared_ptr<Player> player);
	void HandleMove(Protocol::C_MOVE pkt);
	void HandleAttack(shared_ptr<Player> from, uint64 toId);

private:
	bool EnterRoom(shared_ptr<Object> object);
	bool LeaveRoom(shared_ptr<Object> object);

	bool AddObject(shared_ptr<Object> object);
	bool RemoveObject(uint64 objectId);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, shared_ptr<Object>> _objects;
};

extern shared_ptr<Room> gRoom;
