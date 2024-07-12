#pragma once
#include "Creature.h"

class Player : public Creature
{
public:
	Player();
	virtual ~Player();

public:
	weak_ptr<class GameSession> session;
};
