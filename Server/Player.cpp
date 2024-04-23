#include "pch.h"
#include "Player.h"

Player::Player()
{
	playerInfo = new Protocol::PlayerInfo();
}

Player::~Player()
{
	delete playerInfo;
}
