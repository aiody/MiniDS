#include "pch.h"
#include "Brain.h"
#include "Monster.h"
#include "BehaviorTree.h"

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
            return _isBattleMode;
        });

    BTSelectorRef battleModeSelector = make_shared<BTSelector>();

    BTSequenceRef checkBattleModeSequence = make_shared<BTSequence>();
    BTConditionRef isTooFarToAttack = make_shared<BTCondition>([this]() { return false; });
    BTActionRef offBattleMode = make_shared<BTAction>([this]()
        {
            cout << "Battle mode is end." << endl;
            _isBattleMode = false;
        });

    BTSequenceRef avoidWhileCoolingSequence = make_shared<BTSequence>();
    BTConditionRef isAttackCooling = make_shared<BTCondition>([this]() { return false; });
    BTActionRef avoidWithPlayerWhileCooling = make_shared<BTAction>([this]()
        {
            cout << "Run away!" << endl;
        });

    BTSequenceRef attackSequence = make_shared<BTSequence>();
    BTConditionRef isCloseEnoughWithPlayer = make_shared<BTCondition>([this]() { return false; });
    BTActionRef attackPlayer = make_shared<BTAction>([]()
        {
            cout << "Attack!!!" << endl;
        });
    BTActionRef setAttackCooldown = make_shared<BTAction>([]()
        {
            cout << "Set attack cooldown" << endl;
        });

    BTActionRef chasePlayer = make_shared<BTAction>([this]()
        {
            cout << "Chase the Player" << endl;
            _owner->Move();
        });

    BTSelectorRef noneBattleModeSelector = make_shared<BTSelector>();

    BTSequenceRef avoidSequence = make_shared<BTSequence>();
    BTConditionRef isTooCloseWithPlayer = make_shared<BTCondition>([this]()
        {
            return _owner->IsTooCloseWithPlayer(_safe_dist);
        });
    BTSelectorRef selectorAvoid = make_shared<BTSelector>();
    BTConditionRef isFarEnoughWithPlayer = make_shared<BTCondition>([this]()
        {
            return !_owner->IsTooCloseWithPlayer(_stop_run_away_dist);
        });
    BTActionRef avoidPlayer = make_shared<BTAction>([this]()
        {
            cout << "It's too close! Avoid the Player" << endl;
            _owner->RunAway(_stop_run_away_dist);
        });

    BTSequenceRef wanderSequence = make_shared<BTSequence>();
    BTActionRef wanderWait = make_shared<BTAction>([]()
        {
            cout << "Wait." << endl;
        });
    BTActionRef wanderSetDest = make_shared<BTAction>([]()
        {
            cout << "Set destination." << endl;
        });
    BTActionRef wanderMoveToDest = make_shared<BTAction>([]()
        {
            cout << "Move to destination." << endl;
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
    avoidSequence->AddChild(selectorAvoid);
    selectorAvoid->AddChild(isFarEnoughWithPlayer);
    selectorAvoid->AddChild(avoidPlayer);

    wanderSequence->AddChild(wanderWait);
    wanderSequence->AddChild(wanderSetDest);
    wanderSequence->AddChild(wanderMoveToDest);
}

void Brain::Run()
{
    root->run();
}
