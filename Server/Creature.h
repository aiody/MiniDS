#pragma once
#include "Object.h"

class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();

public:
	Protocol::CreatureInfo* creatureInfo;
	Protocol::StatInfo* statInfo;
};
