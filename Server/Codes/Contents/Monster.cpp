#include "pch.h"
#include "Monster.h"
#include "Brain.h"
#include "Player.h"
#include "Room.h"
#include "Vector3.h"

Monster::Monster()
{
	creatureInfo->set_creature_type(Protocol::CREATURE_TYPE_MONSTER);
}

Monster::~Monster()
{
	_brain = nullptr;
}

shared_ptr<Monster> Monster::GetMonsterRef()
{
	return static_pointer_cast<Monster>(shared_from_this());
}

void Monster::Start()
{
	_brain = make_shared<Brain>(GetMonsterRef());
	_brain->Init();
}

void Monster::UpdateTick()
{
	if (_brain != nullptr)
		_brain->Run();
}

void Monster::OnDead()
{
	Creature::OnDead();
	_brain = nullptr;
}

bool Monster::Move(Vector3 targetPos, float speed)
{
	if (creatureInfo->state() != Protocol::CREATURE_STATE_WALK
		&& creatureInfo->state() != Protocol::CREATURE_STATE_RUN)
		return false;

	Vector3 myPos(posInfo->x(), posInfo->y(), targetPos.z);

	Vector3 dir = (targetPos - myPos).normalize() * speed;
	Vector3 destPos = myPos + dir;
	Vector3 lookDir = myPos - destPos;

	posInfo->set_x(destPos.x);
	posInfo->set_y(destPos.y);
	posInfo->set_z(posInfo->z());
	bool getTarget = (targetPos - myPos).length() < 30.f;
	if (getTarget == false)
		posInfo->set_dir(GetMoveDir(lookDir));

	BroadcastMove();

	return getTarget;
}

Protocol::MoveDir Monster::GetMoveDir(Vector3 dirVector)
{
	Vector3 trimZVector(dirVector.y, dirVector.x, 0.0f); // 위쪽이 x 오른쪽이 y 여서 좌표 바꿈
	Vector3 targetVector = trimZVector.normalize();
	Vector3 rightUpVector(1.0f, 1.0f, 0.0f);
	Vector3 rightDownVector(1.0f, -1.0f, 0.0f);

	Protocol::MoveDir Dir = Protocol::MOVE_DIR_NONE;
	if (0.0f > targetVector.dot(rightUpVector))
	{
		if (0.0f > targetVector.dot(rightDownVector))
			Dir = Protocol::MOVE_DIR_RIGHT;
		else
			Dir = Protocol::MOVE_DIR_UP;
	}
	else
	{
		if (0.0f > targetVector.dot(rightDownVector))
			Dir = Protocol::MOVE_DIR_DOWN;
		else
			Dir = Protocol::MOVE_DIR_LEFT;
	}
	return Dir;
}

