#pragma once
#include "Creature.h"

class Player;

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();
	shared_ptr<Monster> GetMonsterRef();

	virtual void Start();
	virtual void UpdateTick();

	void Move();
	void RunAway(float runAwayDist);
	bool IsTooCloseWithPlayer(float runAwayDist) const;
	bool IsFarEnoughWithPlayer(float runAwayDist) const;

private:
	void BroadcastMove();

protected:
	shared_ptr<class Brain> _brain;
	float _speed = 25.f;
};
