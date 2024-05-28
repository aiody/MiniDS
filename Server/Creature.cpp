#include "pch.h"
#include "Creature.h"

Creature::Creature()
{
	objectInfo->set_object_type(Protocol::OBJECT_TYPE_CREATURE);
	creatureInfo = new Protocol::CreatureInfo();
	objectInfo->set_allocated_creature_info(creatureInfo);
	statInfo = new Protocol::StatInfo();
	creatureInfo->set_allocated_stat_info(statInfo);
}

Creature::~Creature()
{
}
