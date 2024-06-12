#pragma once
#include "BehaviorTree.h"
#include "Vector3.h"

class Player;
class Monster;
class BTSequence;

class Brain
{
public:
	Brain(shared_ptr<Monster> owner);
	virtual ~Brain();

	void Init();
	void Run();

protected:
	bool IsBattleMode();
	bool IsTooFarToAttack();
	bool IsAttackCooling();
	BTNodeStatus ChaseAndAttackPlayer();
	bool IsTooCloseWithPlayer();
	BTNodeStatus SetAvoidDest();
	BTNodeStatus AvoidWhileCooling();
	BTNodeStatus MoveToDest();
	BTNodeStatus SetDestForWandering();
	BTNodeStatus Wait();

protected:
	shared_ptr<Monster> _owner;
	shared_ptr<BTSequence> root;

private:
	class Vector3 _destPos;
	bool _isBattleMode = true;
	float _safe_dist = 300.0f;
	float _stop_run_away_dist = 500.0f;
	float _see_target_dist = 1000.0f;
	float _attack_range = 30.0f;
	float _waitUntil = 0.0f;
	float _attackCooldown = 3.0f;
	float _attackCooldownUntil = 0.0f;
};
