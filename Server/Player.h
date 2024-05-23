#pragma once

class Player
{
public:
	Player();
	virtual ~Player();

public:
	weak_ptr<class GameSession> session;
	atomic<weak_ptr<class Room>> room;

public:
	Protocol::PlayerInfo* playerInfo;
	Protocol::StatInfo* statInfo;
};
