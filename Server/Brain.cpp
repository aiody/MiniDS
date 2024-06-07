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
    BTConditionRef isBattleMode = make_shared<BTCondition>([this]() { return false; });

    BTSelectorRef battleModeSelector = make_shared<BTSelector>();

    BTSequenceRef checkBattleModeSequence = make_shared<BTSequence>();
    BTConditionRef isTooFarToAttack = make_shared<BTCondition>([this]() { return false; });
    BTActionRef offBattleMode = make_shared<BTAction>([this]()
        {
            cout << "Battle mode is end." << endl;
            _isBattleMode = false;
            return BTNodeStatus::Success;
        });

    BTSequenceRef avoidWhileCoolingSequence = make_shared<BTSequence>();
    BTConditionRef isAttackCooling = make_shared<BTCondition>([this]() { return false; });
    BTActionRef avoidWithPlayerWhileCooling = make_shared<BTAction>([this]()
        {
            cout << "Run away!" << endl;
            return BTNodeStatus::Success;
        });

    BTSequenceRef attackSequence = make_shared<BTSequence>();
    BTConditionRef isCloseEnoughWithPlayer = make_shared<BTCondition>([this]() { return false; });
    BTActionRef attackPlayer = make_shared<BTAction>([]()
        {
            cout << "Attack!!!" << endl;
            return BTNodeStatus::Success;
        });
    BTActionRef setAttackCooldown = make_shared<BTAction>([]()
        {
            cout << "Set attack cooldown" << endl;
            return BTNodeStatus::Success;
        });

    BTActionRef chasePlayer = make_shared<BTAction>([this]()
        {
            cout << "Chase the Player" << endl;
            return ChasePlayer();
        });

    BTSelectorRef noneBattleModeSelector = make_shared<BTSelector>();

    BTSequenceRef avoidSequence = make_shared<BTSequence>();
    BTConditionRef isTooCloseWithPlayer = make_shared<BTCondition>([this]()
        {
            cout << "Check is too close with player." << endl;
            return IsTooCloseWithPlayer();
        });
    BTActionRef setAvoidDest = make_shared<BTAction>([this]()
        {
            cout << "Set avoid Destination." << endl;
            return SetAvoidDest();
        });

    BTActionRef avoidPlayer = make_shared<BTAction>([this]()
        {
            cout << "Avoiding the Player" << endl;
            return MoveToDest();
        });

    BTSequenceRef wanderSequence = make_shared<BTSequence>();
    BTActionRef wanderWait = make_shared<BTAction>([this]()
        {
            cout << "Wait." << endl;
            return Wait();
        },
        [this]() {
            _waitUntil = ::GetTickCount64() + (Utils::GetRandom(2.f, 4.f) * 1000.f);
            return BTNodeStatus::Running;
        });
    BTActionRef wanderSetDest = make_shared<BTAction>([this]()
        {
            cout << "Set destination." << endl;
            return SetDestForWandering();
        });
    BTActionRef wanderMoveToDest = make_shared<BTAction>([this]()
        {
            cout << "Move to destination." << endl;
            return MoveToDest();
        });

    root->AddChild(rootSelector);

    rootSelector->AddChild(battleModeSequence);
    rootSelector->AddChild(noneBattleModeSelector);

    battleModeSequence->AddChild(isBattleMode);
    battleModeSequence->AddChild(battleModeSelector);

    battleModeSelector->AddChild(checkBattleModeSequence);
    battleModeSelector->AddChild(avoidWhileCoolingSequence);
    battleModeSelector->AddChild(attackSequence);
    battleModeSelector->AddChild(chasePlayer);

    checkBattleModeSequence->AddChild(isTooFarToAttack);
    checkBattleModeSequence->AddChild(offBattleMode);

    avoidWhileCoolingSequence->AddChild(isAttackCooling);
    avoidWhileCoolingSequence->AddChild(avoidWithPlayerWhileCooling);

    attackSequence->AddChild(isCloseEnoughWithPlayer);
    attackSequence->AddChild(attackPlayer);
    attackSequence->AddChild(setAttackCooldown);

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

BTNodeStatus Brain::ChasePlayer()
{
    shared_ptr<Room> myRoom = _owner->room.load().lock();
    if (myRoom == nullptr)
        return BTNodeStatus::Failure;

    shared_ptr<Player> target = myRoom->FindPlayer();
    if (target == nullptr)
        return BTNodeStatus::Failure;

    Vector3 targetPos(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());

    return _owner->Move(targetPos) ? BTNodeStatus::Success : BTNodeStatus::Running;
}

bool Brain::IsTooCloseWithPlayer()
{
    shared_ptr<Room> myRoom = _owner->room.load().lock();
    if (myRoom == nullptr)
        return false;

    for (auto& player : myRoom->FindPlayers())
    {
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
    //cout << "TargetPos : " << targetPos.x << ", " << targetPos.y << ", " << targetPos.z << endl;
    Vector3 myPos(_owner->posInfo->x(), _owner->posInfo->y(), targetPos.z);
    Vector3 destPos = (myPos - targetPos).normalize() * _stop_run_away_dist;
    _destPos = destPos;
    return BTNodeStatus::Success;
}

BTNodeStatus Brain::MoveToDest()
{
    return _owner->Move(_destPos) ? BTNodeStatus::Success : BTNodeStatus::Running;
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
    if (::GetTickCount64() < _waitUntil)
        return BTNodeStatus::Running;

    return BTNodeStatus::Success;
}
