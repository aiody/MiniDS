#pragma once

enum BTNodeStatus
{
	Ready,
	Running,
	Success,
	Failure,
};

// base 노드
class BTNode
{
public:
	virtual ~BTNode() {}
	
	BTNodeStatus Init() { return BTNodeStatus::Running; }
	void Reset() { _status = BTNodeStatus::Ready; }
	virtual BTNodeStatus Run() = 0;

protected:
	BTNodeStatus _status = BTNodeStatus::Ready;
};

// 자식을 가질 수 있는 노드
class BTComposite : public BTNode
{
public:
	virtual ~BTComposite() {}

	void AddChild(shared_ptr<BTNode> child)
	{
		_children.push_back(child);
	}

	void ResetChildren()
	{
		_runningIdx = 0;
		for (auto& child : _children)
		{
			child->Reset();
		}
	}

protected:
	std::vector<shared_ptr<BTNode>> _children;
	int _runningIdx = 0;
};

// AND 노드
class BTSequence : public BTComposite
{
public:
	virtual BTNodeStatus Run() override
	{
		while (_runningIdx < _children.size())
		{
			shared_ptr<BTNode> child = _children[_runningIdx];
			BTNodeStatus status = child->Run();
			switch (status)
			{
			case BTNodeStatus::Running:
				return status;
			case BTNodeStatus::Success:
				_runningIdx++;
				break;
			case BTNodeStatus::Failure:
				ResetChildren();
				return status;
			}
		}

		ResetChildren();
		return BTNodeStatus::Success;
	}
};

// OR 노드
class BTSelector : public BTComposite
{
public:
	virtual BTNodeStatus Run() override
	{
		while (_runningIdx < _children.size())
		{
			shared_ptr<BTNode> child = _children[_runningIdx];
			BTNodeStatus status = child->Run();
			switch (status)
			{
			case BTNodeStatus::Running:
				return status;
			case BTNodeStatus::Success:
				ResetChildren();
				return status;
			case BTNodeStatus::Failure:
				_runningIdx++;
				break;
			}
		}

		ResetChildren();
		return BTNodeStatus::Failure;
	}
};

// 조건문 노드
using ConditionFuncType = function<bool()>;
class BTCondition : public BTNode
{
public:
	BTCondition(ConditionFuncType func) : _func(func) {}

	virtual BTNodeStatus Run() override
	{
		return _func() == true ? BTNodeStatus::Success : BTNodeStatus::Failure;
	}

private:
	ConditionFuncType _func;
};

// 작업 노드
using ActionFuncType = function<BTNodeStatus()>;
class BTAction : public BTNode
{
public:
	BTAction(ActionFuncType func, ActionFuncType init = nullptr) : _func(func), _init(init) {}

	virtual BTNodeStatus Run() override
	{
		if (_status == BTNodeStatus::Ready)
			_status = _init == nullptr ? Init() : _init();

		if (_status == BTNodeStatus::Running)
			_status = _func();

		return _status;
	}

private:
	ActionFuncType _func;
	ActionFuncType _init;
};

using BTSequenceRef = shared_ptr<BTSequence>;
using BTSelectorRef = shared_ptr<BTSelector>;
using BTConditionRef = shared_ptr<BTCondition>;
using BTActionRef = shared_ptr<BTAction>;
