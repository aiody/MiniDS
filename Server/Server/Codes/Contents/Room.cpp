#include "pch.h"
#include "Room.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "ObjectUtils.h"
#include "Creature.h"

shared_ptr<Room> gRoom = make_shared<Room>();

Room::Room()
{
}

Room::~Room()
{
}

void Room::Start()
{
	gJobQueue->Push(make_shared<Job>(gRoom, &Room::UpdateTick));

	{
		shared_ptr<Monster> monster = ObjectUtils::CreateMonster();
		EnterRoom(monster);
		monster->Start();
	}
}

void Room::UpdateTick()
{
	for (auto& item : _objects)
	{
		shared_ptr<Object> object = item.second;
		if (object->objectInfo->has_creature_info() &&
			object->objectInfo->creature_info().creature_type() & Protocol::CREATURE_TYPE_MONSTER)
		{
			shared_ptr<Monster> monster = static_pointer_cast<Monster>(object);
			monster->UpdateTick();
		}
	}

	shared_ptr<Job> job = make_shared<Job>(shared_from_this(), &Room::UpdateTick);
	gJobTimer->Reserve(100, job);
}

shared_ptr<Player> Room::FindPlayer()
{
	for (auto& item : _objects)
	{
		shared_ptr<Object> object = item.second;
		if (object->objectInfo->has_creature_info() &&
			object->objectInfo->creature_info().creature_type() & Protocol::CREATURE_TYPE_PLAYER)
		{
			shared_ptr<Player> player = static_pointer_cast<Player>(object);
			return player;
		}
	}
	return nullptr;
}

vector<shared_ptr<Player>> Room::FindPlayers()
{
	vector<shared_ptr<Player>> players;
	for (auto& item : _objects)
	{
		shared_ptr<Object> object = item.second;
		if (object->objectInfo->has_creature_info() &&
			object->objectInfo->creature_info().creature_type() & Protocol::CREATURE_TYPE_PLAYER)
		{
			shared_ptr<Player> player = static_pointer_cast<Player>(object);
			players.push_back(player);
		}
	}
	return players;
}

bool Room::HasObject(uint64 id)
{
	return _objects.find(id) == _objects.end();
}

bool Room::HandleEnterPlayer(shared_ptr<Player> player)
{
	return EnterRoom(player);
}

bool Room::HandleLeavePlayer(shared_ptr<Player> player)
{
	return LeaveRoom(player);
}

void Room::HandleMove(Protocol::C_MOVE pkt)
{
	const uint64 id = pkt.object_id();
	if (_objects.find(id) == _objects.end())
		return;

	shared_ptr<Object> object = _objects[id];
	if (object->IsPlayer())
	{
		shared_ptr<Player> player = static_pointer_cast<Player>(object);
		player->posInfo->MergeFrom(pkt.pos_info());
		player->creatureInfo->set_state(pkt.state());
	}

	{
		Protocol::S_MOVE movePkt;
		{
			movePkt.set_object_id(id);
			Protocol::PosInfo* posInfo = movePkt.mutable_pos_info();
			posInfo->CopyFrom(pkt.pos_info());
			movePkt.set_state(pkt.state());
		}
		shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}
}

void Room::HandleAttack(uint64 fromId, uint64 toId)
{
	if (_objects.find(fromId) == _objects.end())
		return;
	if (_objects.find(toId) == _objects.end())
		return;

	// hp 감소
	shared_ptr<Object> from = _objects[fromId];
	shared_ptr<Object> to = _objects[toId];

	// 피격 처리
	shared_ptr<Creature> attacker = static_pointer_cast<Creature>(from);
	shared_ptr<Creature> hitCreature = static_pointer_cast<Creature>(to);
	float hpOfHitCreature = hitCreature->statInfo->hp();
	float damage = attacker->statInfo->damage();
	float remainHp = ::max(hpOfHitCreature - damage, 0.0f);
	hitCreature->statInfo->set_hp(remainHp);
	hitCreature->SetTarget(attacker);

	cout << "[Game] ( " << fromId << " ) attacked ( " << toId << " ) with " << damage << " damage. HP " << remainHp << " left." << endl;

	if (remainHp <= 0)
	{
		cout << "[Game] ( " << toId << " ) is dead." << endl;
		attacker->SetTarget(nullptr);
		hitCreature->OnDead();
		Protocol::S_DEATH deathPkt;
		{
			deathPkt.set_from(from->objectInfo->object_id());
			deathPkt.set_to(toId);
			deathPkt.set_damage(damage);
		}
		shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(deathPkt);
		Broadcast(sendBuffer);
	}
	else
	{
		Protocol::S_HIT hitPkt;
		{
			hitPkt.set_from(from->objectInfo->object_id());
			hitPkt.set_to(toId);
			hitPkt.set_damage(damage);
		}
		shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(hitPkt);
		Broadcast(sendBuffer);
	}
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBuffer, uint64 exceptId)
{
	for (auto& item : _objects)
	{
		shared_ptr<Object> object = item.second;
		if (object->IsPlayer())
		{
			shared_ptr<Player> player = static_pointer_cast<Player>(object);
			if (player->objectInfo->object_id() == exceptId)
				continue;
			if (shared_ptr<GameSession> session = player->session.lock())
				session->Send(sendBuffer);
		}
	}
}

bool Room::EnterRoom(shared_ptr<Object> object)
{
	cout << "[Game] ( " << object->objectInfo->object_id() << " ) entered room." << endl;

	bool success = AddObject(object);

	object->posInfo->set_x(Utils::GetRandom(-1000.f, 1000.f));
	object->posInfo->set_y(Utils::GetRandom(-1000.f, 1000.f));
	object->posInfo->set_z(230.f);
	if (object->IsPlayer())
	{
		auto player = static_pointer_cast<Player>(object);
		player->statInfo->set_max_hp(175.f);
		player->statInfo->set_hp(175.f);
		player->statInfo->set_damage(25.f);
	}
	else
	{
		auto creature = static_pointer_cast<Creature>(object);
		creature->statInfo->set_max_hp(250.f);
		creature->statInfo->set_hp(250.f);
		creature->statInfo->set_damage(33.f);
	}

	// 해당 플레이어에게 게임에 접속함을 알림
	if (object->IsPlayer())
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		Protocol::ObjectInfo* playerInfo = new Protocol::ObjectInfo();
		playerInfo->CopyFrom(*object->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		if (auto session = static_pointer_cast<Player>(object)->session.lock())
		{
			SEND_PACKET(enterGamePkt);
		}
	}

	// 현재 플레이어에게 다른 플레이어가 있음을 알림
	if (object->IsPlayer())
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _objects)
		{
			shared_ptr<Object> obj = item.second;
			if (obj->objectInfo->object_id() == object->objectInfo->object_id())
				continue;

			Protocol::ObjectInfo* objectInfo = spawnPkt.add_objects();
			objectInfo->CopyFrom(*obj->objectInfo);
		}

		if (auto session = static_pointer_cast<Player>(object)->session.lock())
		{
			SEND_PACKET(spawnPkt);
		}
	}

	// 다른 플레이어에게 현재 플레이어가 게임에 접속함을 알림
	{
		Protocol::S_SPAWN spawnPkt;
		Protocol::ObjectInfo* objectInfo = spawnPkt.add_objects();
		objectInfo->CopyFrom(*object->objectInfo);

		shared_ptr<SendBuffer> spawnBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		gJobQueue->Push(make_shared<Job>(gRoom, &Room::Broadcast, spawnBuffer, object->objectInfo->object_id()));
	}

	return success;
}

bool Room::LeaveRoom(shared_ptr<Object> object)
{
	if (object == nullptr)
		return false;

	cout << "[Game] ( " << object->objectInfo->object_id() << " ) left room." << endl;

	const uint64 id = object->objectInfo->object_id();
	bool success = RemoveObject(id);

	// 해당 플레이어에게 게임에 퇴장함을 알림
	if (object->IsPlayer())
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;
		if (auto session = static_pointer_cast<Player>(object)->session.lock())
		{
			SEND_PACKET(leaveGamePkt);
		}
	}

	// 다른 플레이어에게 현재 플레이어가 게임에서 퇴장한 사실을 알림
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(id);

		shared_ptr<SendBuffer> despawnBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		gJobQueue->Push(make_shared<Job>(gRoom, &Room::Broadcast, despawnBuffer, id));

		// LeavePlayer에서 room에서 빼버렸기 때문에 Broadcast 대상에 들어가지 않아 다시 보냄
		if (object->IsPlayer())
		{
			if (auto session = static_pointer_cast<Player>(object)->session.lock())
				session->Send(despawnBuffer);
		}
	}

	if (auto session = static_pointer_cast<Player>(object)->session.lock())
	{
		session->Disconnect(L"User");
	}

	return true;
}

bool Room::AddObject(shared_ptr<Object> object)
{
	// 중복됨
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(::make_pair(object->objectInfo->object_id(), object));

	object->room.store(shared_from_this());

	return true;
}

bool Room::RemoveObject(uint64 objectId)
{
	// 존재하는지 체크
	if (_objects.find(objectId) == _objects.end())
		return false;
	
	shared_ptr<Object> object = _objects[objectId];
	if (object->IsPlayer())
	{
		shared_ptr<Player> player = static_pointer_cast<Player>(object);
		player->room.store(weak_ptr<Room>());
	}

	_objects.erase(objectId);

	return true;
}
