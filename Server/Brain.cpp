#include "pch.h"
#include "Brain.h"
#include "BehaviorTree.h"

Brain::Brain()
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
    BTConditionRef isBattleMode = make_shared<BTCondition>(true);

    BTSelectorRef battleModeSelector = make_shared<BTSelector>();

    BTSequenceRef checkBattleModeSequence = make_shared<BTSequence>();
    BTConditionRef isTooFarToAttack = make_shared<BTCondition>(false);
    BTActionRef offBattleMode = make_shared<BTAction>("Battle mode is end.");

    BTSequenceRef avoidWhileCoolingSequence = make_shared<BTSequence>();
    BTConditionRef isAttackCooling = make_shared<BTCondition>(false);
    BTActionRef avoidWithPlayerWhileCooling = make_shared<BTAction>("Run away!");

    BTSequenceRef attackSequence = make_shared<BTSequence>();
    BTConditionRef isCloseEnoughWithPlayer = make_shared<BTCondition>(false);
    BTActionRef attackPlayer = make_shared<BTAction>("Attack!!!");
    BTActionRef setAttackCooldown = make_shared<BTAction>("Set attack cooldown");

    BTActionRef chasePlayer = make_shared<BTAction>("Chase the Player");

    BTSelectorRef noneBattleModeSelector = make_shared<BTSelector>();

    BTSequenceRef avoidSequence = make_shared<BTSequence>();
    BTConditionRef isTooCloseWithPlayer = make_shared<BTCondition>(false);
    BTSelectorRef selectorAvoid = make_shared<BTSelector>();
    BTConditionRef isFarEnoughWithPlayer = make_shared<BTCondition>(false);
    BTActionRef avoidPlayer = make_shared<BTAction>("It's too close! Avoid the Player");

    BTSequenceRef wanderSequence = make_shared<BTSequence>();
    BTActionRef wanderWait = make_shared<BTAction>("Wait.");
    BTActionRef wanderSetDest = make_shared<BTAction>("Set destination.");
    BTActionRef wanderMoveToDest = make_shared<BTAction>("Move to destination.");

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
