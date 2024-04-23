#pragma once

class ObjectUtils
{
public:
	static shared_ptr<class Player> CreatePlayer(shared_ptr<class GameSession> session);

private:
	static atomic<int64> s_idGenerator;
};
