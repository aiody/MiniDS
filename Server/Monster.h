#pragma once
#include "Creature.h"

class Player;
class Vector3;

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();
	shared_ptr<Monster> GetMonsterRef();

	virtual void Start();
	virtual void UpdateTick();

	bool Move(Vector3 targetPos);

private:
	void BroadcastMove();

protected:
	shared_ptr<class Brain> _brain;
	float _speed = 25.f;
};
