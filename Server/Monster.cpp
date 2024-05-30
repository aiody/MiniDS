#include "pch.h"
#include "Monster.h"
#include "Brain.h"

Monster::Monster()
{
	creatureInfo->set_creature_type(Protocol::CREATURE_TYPE_MONSTER);
	_brain = make_shared<Brain>();
	_brain->Init();
}

Monster::~Monster()
{
}

void Monster::UpdateTick()
{
	//cout << "Monster Update Tick" << endl;
	_brain->Run();
}
