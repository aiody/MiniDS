#include "pch.h"
#include "Brain.h"
#include "Monster.h"
#include "Room.h"
#include "Player.h"
#include "Vector3.h"
#include "Utils.h"

Brain::Brain(shared_ptr<class Monster> owner) : _owner(owner)
{
	root = make_shared<BTSequence>();
}

Brain::~Brain()
{
}

void Brain::Init()
{
    BTSelectorRef rootSelector = make_shared<BTSelector>();

    BTSequenceRef battleModeSequence = make_shared<BTSequence>();
    BTConditionRef isBattleMode = make_shared<BTCondition>([this]()
        {
            return IsBattleMode();
        });

    BTSelectorRef battleModeSelector = make_shared<BTSelector>();

    BTSequenceRef avoidWhileCoolingSequence = make_shared<BTSequence>();
    BTConditionRef isAttackCooling = make_shared<BTCondition>([this]()
        {
            bool result = IsAttackCooling();
            return IsAttackCooling();
        });
    BTActionRef setAvoidDir = make_shared<BTAction>([this]()
        {
            return SetAvoidDir();
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Set direction to avoid." << endl;
            return BTNodeStatus::Running;
        });
    BTActionRef avoidWithPlayerWhileCooling = make_shared<BTAction>([this]()
        {
            return AvoidWhileCooling();
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Run away!" << endl;

            if (IsDead())
                return BTNodeStatus::Failure;

            _owner->SetState(Protocol::CREATURE_STATE_RUN);
            return BTNodeStatus::Running;
        });

    BTActionRef chaseAndAttackPlayer = make_shared<BTAction>([this]()
        {
            return ChaseAndAttackPlayer();
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Chase the Player." << endl;

            if (IsDead())
                return BTNodeStatus::Failure;

            _owner->SetState(Protocol::CREATURE_STATE_RUN);
            return BTNodeStatus::Running;
        });


    BTSelectorRef noneBattleModeSelector = make_shared<BTSelector>();

    BTSequenceRef avoidSequence = make_shared<BTSequence>();
    BTConditionRef isTooCloseWithPlayer = make_shared<BTCondition>([this]()
        {
            return IsTooCloseWithPlayer();
        });
    BTActionRef setAvoidDest = make_shared<BTAction>([this]()
        {
            return SetAvoidDest();
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Set destination to avoid." << endl;
            return BTNodeStatus::Running;
        });

    BTActionRef avoidPlayer = make_shared<BTAction>([this]()
        {
            return MoveToDest(_speed_run);
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Avoid the Player." << endl;

            if (IsDead())
                return BTNodeStatus::Failure;

            _owner->SetState(Protocol::CREATURE_STATE_RUN);
            return BTNodeStatus::Running;
        });

    BTSequenceRef wanderSequence = make_shared<BTSequence>();
    BTActionRef wanderWait = make_shared<BTAction>([this]()
        {
            return Wait();
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Wait." << endl;

            if (IsDead())
                return BTNodeStatus::Failure;

            _waitUntil = ::GetTickCount64() + (Utils::GetRandom(2.f, 4.f) * 1000.f);
            _owner->SetState(Protocol::CREATURE_STATE_IDLE);
            return BTNodeStatus::Running;
        });
    BTActionRef wanderSetDest = make_shared<BTAction>([this]()
        {
            return SetDestForWandering();
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Set destination to wander." << endl;
            return BTNodeStatus::Running;
        });
    BTActionRef wanderMoveToDest = make_shared<BTAction>([this]()
        {
            return MoveToDest(_speed_walk);
        },
        [this]() {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Move to destination for wandering." << endl;

            if (IsDead())
                return BTNodeStatus::Failure;

            _owner->SetState(Protocol::CREATURE_STATE_WALK);
            return BTNodeStatus::Running;
        });

    root->AddChild(rootSelector);

    rootSelector->AddChild(battleModeSequence);
    rootSelector->AddChild(noneBattleModeSelector);

    battleModeSequence->AddChild(isBattleMode);
    battleModeSequence->AddChild(battleModeSelector);

    battleModeSelector->AddChild(avoidWhileCoolingSequence);
    battleModeSelector->AddChild(chaseAndAttackPlayer);

    avoidWhileCoolingSequence->AddChild(isAttackCooling);
    avoidWhileCoolingSequence->AddChild(setAvoidDir);
    avoidWhileCoolingSequence->AddChild(avoidWithPlayerWhileCooling);

    noneBattleModeSelector->AddChild(avoidSequence);
    noneBattleModeSelector->AddChild(wanderSequence);

    avoidSequence->AddChild(isTooCloseWithPlayer);
    avoidSequence->AddChild(setAvoidDest);
    avoidSequence->AddChild(avoidPlayer);

    wanderSequence->AddChild(wanderWait);
    wanderSequence->AddChild(wanderSetDest);
    wanderSequence->AddChild(wanderMoveToDest);
}

void Brain::Run()
{
    root->Run();
}

bool Brain::IsDead()
{
    return _owner->creatureInfo->state() == Protocol::CREATURE_STATE_DEATH;
}

bool Brain::IsBattleMode()
{
    return _owner->_target != nullptr;
}

bool Brain::IsTooFarToAttack()
{
    shared_ptr<Room> myRoom = _owner->room.load().lock();
    if (myRoom == nullptr)
        return false;

    shared_ptr<Creature> target = _owner->_target;
    if (target == nullptr)
        return false;

    Vector3 targetPos(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());
    Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), target->posInfo->z());
    float dist = (targetPos - myPos).length();
    if (dist >= _see_target_dist)
    {
        return true;
    }

    return false;
}

bool Brain::IsAttackCooling()
{
    if (::GetTickCount64() < _attackCooldownUntil)
        return true;

    return false;
}

BTNodeStatus Brain::ChaseAndAttackPlayer()
{
    shared_ptr<Room> myRoom = _owner->room.load().lock();
    if (myRoom == nullptr)
        return BTNodeStatus::Failure;

    if (_owner->_target == nullptr)
        return BTNodeStatus::Failure;

    if (myRoom->HasObject(_owner->_target->objectInfo->object_id()))
    {
        _owner->_target = nullptr;
        return BTNodeStatus::Failure;
    }

    shared_ptr<Player> target = _owner->_target->IsPlayer() ? static_pointer_cast<Player>(_owner->_target) : nullptr;
    if (target == nullptr)
        return BTNodeStatus::Failure;

    if (_owner->creatureInfo->state() != Protocol::CREATURE_STATE_ATTACK)
    {
        Vector3 targetPos(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());
        _owner->Move(targetPos, _speed_run);
        Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), target->posInfo->z());

        float dist = (targetPos - myPos).length();
        if (dist >= _see_target_dist)
        {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Battle mode is end." << endl;
            _owner->SetTarget(nullptr);
            return BTNodeStatus::Failure;
        }

        if (dist < _attack_range)
        {
            cout << "[AI] ( " << _owner->objectInfo->object_id() << " ) Attack!" << endl;
            _owner->SetState(Protocol::CREATURE_STATE_ATTACK);
            _attackCooldownUntil = ::GetTickCount64() + (_attackCooldown * 1000.0f);
            _attackUntil = ::GetTickCount64() + (_attackLength * 1000.0f);
            return BTNodeStatus::Running;
        }
    }
    else
    {
        if (::GetTickCount64() > _attackUntil)
            return BTNodeStatus::Success;
        else
            return BTNodeStatus::Running;
    }

    return BTNodeStatus::Running;
}

bool Brain::IsTooCloseWithPlayer()
{
    shared_ptr<Room> myRoom = _owner->room.load().lock();
    if (myRoom == nullptr)
        return false;

    for (auto& player : myRoom->FindPlayers())
    {
        if (player->creatureInfo->state() == Protocol::CREATURE_STATE_DEATH)
            continue;

        Vector3 targetPos(player->posInfo->x(), player->posInfo->y(), player->posInfo->z());
        Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), player->posInfo->z());
        float dist = (targetPos - myPos).length();
        if (dist < _safe_dist)
        {
            return true;
        }
    }

    return false;
}

BTNodeStatus Brain::SetAvoidDest()
{
    shared_ptr<Room> myRoom = _owner->room.load().lock();
    if (myRoom == nullptr)
        return BTNodeStatus::Failure;

    shared_ptr<Player> target = nullptr;
    for (auto& player : myRoom->FindPlayers())
    {
        Vector3 targetPos(player->posInfo->x(), player->posInfo->y(), player->posInfo->z());
        Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), player->posInfo->z());
        float dist = (targetPos - myPos).length();
        if (dist < _safe_dist)
        {
            target = player;
            break;
        }
    }
    if (target == nullptr)
        return BTNodeStatus::Failure;

    Vector3 targetPos(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());
    Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), targetPos.z);
    Vector3 destPos = (myPos - targetPos).normalize() * _stop_run_away_dist;
    _destPos = destPos;
    return BTNodeStatus::Success;
}

BTNodeStatus Brain::SetAvoidDir()
{
    shared_ptr<Room> myRoom = _owner->room.load().lock();
    if (myRoom == nullptr)
        return BTNodeStatus::Failure;

    shared_ptr<Player> target = myRoom->FindPlayer();
    if (target == nullptr)
        return BTNodeStatus::Failure;

    Vector3 targetPos(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());
    Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), targetPos.z);
    Vector3 avoidDir = (myPos - targetPos).normalize();
    _destDir = avoidDir;
    return BTNodeStatus::Success;
}

BTNodeStatus Brain::AvoidWhileCooling()
{
    if (::GetTickCount64() >= _attackCooldownUntil)
        return BTNodeStatus::Success;

    Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), _owner->posInfo->z());
    _owner->Move(myPos + _destDir * 1000.0f, _speed_run);

    return BTNodeStatus::Running;
}

BTNodeStatus Brain::MoveToDest(float speed)
{
    if (IsBattleMode())
        return BTNodeStatus::Failure;

    return _owner->Move(_destPos, speed) ? BTNodeStatus::Success : BTNodeStatus::Running;
}

BTNodeStatus Brain::SetDestForWandering()
{
    Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), _owner->posInfo->z());
    Vector3 randomDir(Utils::GetRandom(-1.f, 1.f), Utils::GetRandom(-1.f, 1.f), 0.f);
    float randomDist(Utils::GetRandom(200.f, 500.f));
    _destPos = myPos + (randomDir * randomDist);
    return BTNodeStatus::Success;
}

BTNodeStatus Brain::Wait()
{
    if (IsTooCloseWithPlayer() || IsBattleMode())
        return BTNodeStatus::Failure;

    if (::GetTickCount64() < _waitUntil)
        return BTNodeStatus::Running;

    return BTNodeStatus::Success;
}
