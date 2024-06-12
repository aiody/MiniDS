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

void Monster::SetState(Protocol::CreatureState state)
{
	creatureInfo->set_state(state);
	BroadcastMove();
}

bool Monster::Move(Vector3 targetPos)
{
	Vector3 myPos(posInfo->x(), posInfo->y(), targetPos.z);

	Vector3 dir = (targetPos - myPos).normalize() * _speed;
	Vector3 destPos = myPos + dir;

	posInfo->set_x(destPos.x);
	posInfo->set_y(destPos.y);
	posInfo->set_z(posInfo->z());

	BroadcastMove();

	//cout << "DestPos : " << targetPos.x << ", " << targetPos.y << ", " << targetPos.z << endl;
	//cout << "MyPos : " << myPos.x << ", " << myPos.y << ", " << myPos.z << endl;
	//cout << "Length : " << (targetPos - myPos).length() << endl;
	return (targetPos - myPos).length() < 30.f;
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
