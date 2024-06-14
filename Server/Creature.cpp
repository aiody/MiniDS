#include "pch.h"
#include "Creature.h"
#include "Room.h"

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

void Creature::SetState(Protocol::CreatureState state)
{
	creatureInfo->set_state(state);
	BroadcastMove();
}

void Creature::BroadcastMove()
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
