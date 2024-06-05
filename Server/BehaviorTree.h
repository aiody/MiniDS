#pragma once

// base 노드
class BTNode
{
public:
	virtual ~BTNode() {}

	virtual bool run() = 0;
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

protected:
	std::vector<shared_ptr<BTNode>> _children;
};

// AND 노드
class BTSequence : public BTComposite
{
public:
	virtual bool run() override
	{
		for (auto& child : _children)
		{
			if (!child->run())
				return false;
		}
		return true;
	}
};

// OR 노드
class BTSelector : public BTComposite
{
public:
	virtual bool run() override
	{
		for (auto& child : _children)
		{
			if (child->run())
				return true;
		}
		return false;
	}
};

// 조건문 노드
using ConditionFuncType = function<bool()>;
class BTCondition : public BTNode
{
public:
	BTCondition(ConditionFuncType func) : _func(func) {}

	virtual bool run() override
	{
		return _func();
	}

private:
	ConditionFuncType _func;
};

// 작업 노드
using ActionFuncType = function<void()>;
class BTAction : public BTNode
{
public:
	BTAction(ActionFuncType func) : _func(func) {}

	virtual bool run() override
	{
		_func();
		return true;
	}

private:
	ActionFuncType _func;
};

using BTSequenceRef = shared_ptr<BTSequence>;
using BTSelectorRef = shared_ptr<BTSelector>;
using BTConditionRef = shared_ptr<BTCondition>;
using BTActionRef = shared_ptr<BTAction>;
