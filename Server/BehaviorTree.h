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
		children.push_back(child);
	}

protected:
	std::vector<shared_ptr<BTNode>> children;
};

// AND 노드
class BTSequence : public BTComposite
{
public:
	virtual bool run() override
	{
		for (auto& child : children)
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
		for (auto& child : children)
		{
			if (child->run())
				return true;
		}
		return false;
	}
};

// 조건문 노드
class BTCondition : public BTNode
{
public:
	BTCondition(bool condition) : condition(condition) {}

	virtual bool run() override
	{
		return condition;
	}

private:
	bool condition;
};

// 작업 노드
class BTAction : public BTNode
{
public:
	BTAction(const std::string& name) : name(name) {}

	virtual bool run() override
	{
		cout << "Running action: " << name << endl;
		return true;
	}

private:
	std::string name;
};

using BTSequenceRef = shared_ptr<BTSequence>;
using BTSelectorRef = shared_ptr<BTSelector>;
using BTConditionRef = shared_ptr<BTCondition>;
using BTActionRef = shared_ptr<BTAction>;
