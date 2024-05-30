#pragma once
#include "Creature.h"

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();

	virtual void UpdateTick();

protected:
	shared_ptr<class Brain> _brain;
};
