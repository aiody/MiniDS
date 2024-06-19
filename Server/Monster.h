#pragma once
#include "Creature.h"

class Player;
class Vector3;

class Monster : public Creature
{
	friend class Brain;

public:
	Monster();
	virtual ~Monster();
	shared_ptr<Monster> GetMonsterRef();

	virtual void Start();
	virtual void UpdateTick();

	bool Move(Vector3 targetPos, float speed);
	Protocol::MoveDir GetMoveDir(Vector3 dirVector);

protected:
	shared_ptr<class Brain> _brain;
};
