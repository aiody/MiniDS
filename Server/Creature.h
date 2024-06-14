#pragma once
#include "Object.h"

class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();

	void SetTarget(shared_ptr<class Creature> target) { _target = target; }
	void SetState(Protocol::CreatureState state);

protected:
	void BroadcastMove();

public:
	Protocol::CreatureInfo* creatureInfo;
	Protocol::StatInfo* statInfo;

protected:
	shared_ptr<class Creature> _target;
};
