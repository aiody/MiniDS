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
	//cout << "Monster Update Tick" << endl;
	_brain->Run();
}

void Monster::Move()
{
	shared_ptr<Room> myRoom = room.load().lock();
	if (myRoom == nullptr)
		return;

	shared_ptr<Player> target = myRoom->FindPlayer();
	if (target == nullptr)
		return;

	Vector3 targetPos(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());
	Vector3 myPos(posInfo->x(), posInfo->y(), target->posInfo->z());

	Vector3 dir = (targetPos - myPos).normalize() * _speed;
	Vector3 destPos = myPos + dir;

	posInfo->set_x(destPos.x);
	posInfo->set_y(destPos.y);
	posInfo->set_z(destPos.z);

	BroadcastMove();
}

void Monster::RunAway(float runAwayDist)
{
	shared_ptr<Room> myRoom = room.load().lock();
	if (myRoom == nullptr)
		return;

	shared_ptr<Player> target = _brain->GetTarget();
	if (target == nullptr)
		return;

	Vector3 targetPos(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());
	Vector3 myPos(posInfo->x(), posInfo->y(), target->posInfo->z());
	float dist = (targetPos - myPos).length();
	if (dist >= runAwayDist)
		return;

	// run away
	Vector3 dir = (myPos - targetPos).normalize() * _speed;
	Vector3 destPos = myPos + dir;

	posInfo->set_x(destPos.x);
	posInfo->set_y(destPos.y);
	posInfo->set_z(destPos.z);

	BroadcastMove();
}

bool Monster::IsTooCloseWithPlayer(float runAwayDist) const
{
	shared_ptr<Room> myRoom = room.load().lock();
	if (myRoom == nullptr)
		return false;

	for (auto& player : myRoom->FindPlayers())
	{
		Vector3 targetPos(player->posInfo->x(), player->posInfo->y(), player->posInfo->z());
		Vector3 myPos(posInfo->x(), posInfo->y(), player->posInfo->z());
		float dist = (targetPos - myPos).length();
		if (dist < runAwayDist)
		{
			_brain->SetTarget(player);
			return true;
		}
	}

	return false;
}

void Monster::BroadcastMove()
{
	shared_ptr<Room> myRoom = room.load().lock();
	if (myRoom == nullptr)
		return;

	Protocol::S_MOVE movePkt;
	{
		movePkt.set_object_id(objectInfo->object_id());
		Protocol::PosInfo* pktPosInfo = movePkt.mutable_pos_info();
		pktPosInfo->CopyFrom(*posInfo);
		movePkt.set_state(creatureInfo->state());
	}
	shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	myRoom->Broadcast(sendBuffer);
}
