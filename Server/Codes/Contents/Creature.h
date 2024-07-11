#pragma once
#include "Object.h"

class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();

	void SetTarget(shared_ptr<Creature> target) { _target = target; }
	shared_ptr<Creature> GetTarget() { return _target; }
	void SetState(Protocol::CreatureState state);

	virtual void OnDead();

protected:
	void BroadcastMove();

public:
	Protocol::CreatureInfo* creatureInfo;
	Protocol::StatInfo* statInfo;

protected:
	shared_ptr<Creature> _target;
};
