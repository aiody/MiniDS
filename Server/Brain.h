#pragma once

class Brain
{
public:
	Brain(shared_ptr<class Monster> owner);
	virtual ~Brain();

	void Init();
	void Run();

	void SetTarget(shared_ptr<class Player> target) { _target = target; }
	shared_ptr<class Player> GetTarget() { return _target; };

protected:
	shared_ptr<class Monster> _owner;
	shared_ptr<class BTSequence> root;

private:
	shared_ptr<class Player> _target = nullptr;
	bool _isBattleMode = false;
	float _safe_dist = 300.0f;
	float _stop_run_away_dist = 600.0f;
};
