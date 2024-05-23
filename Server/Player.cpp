#include "pch.h"
#include "Player.h"

Player::Player()
{
	playerInfo = new Protocol::PlayerInfo();
	statInfo = new Protocol::StatInfo();
	playerInfo->set_allocated_stat(statInfo);
}

Player::~Player()
{
	delete playerInfo;
}
