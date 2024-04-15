#pragma once

class Player
{
public:
	uint64 playerId = 0;
	shared_ptr<class GameSession> ownerSession;
};
