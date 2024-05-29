#include "pch.h"
#include "Monster.h"

Monster::Monster()
{
	creatureInfo->set_creature_type(Protocol::CREATURE_TYPE_MONSTER);
}

Monster::~Monster()
{
}

void Monster::UpdateTick()
{
	//cout << "Monster Update Tick" << endl;
}
