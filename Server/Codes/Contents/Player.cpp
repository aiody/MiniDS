#include "pch.h"
#include "Player.h"

Player::Player()
{
	creatureInfo->set_creature_type(Protocol::CREATURE_TYPE_PLAYER);
	_isPlayer = true;
}

Player::~Player()
{
}
