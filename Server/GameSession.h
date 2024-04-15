#pragma once
#include "Session.h"

class GameSession : public PacketSession
{
public:
	GameSession();
	~GameSession();

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

private:
	uint64 GenPlayerId();

public:
	shared_ptr<class Player> curPlayer;
};
