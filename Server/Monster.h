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

	void SetState(Protocol::CreatureState state);
	void SetTarget(shared_ptr<class Creature> target) { _target = target; }
	bool Move(Vector3 targetPos);

private:
	void BroadcastMove();

protected:
	shared_ptr<class Brain> _brain;
	float _speed = 25.f;
	shared_ptr<class Creature> _target;
};
