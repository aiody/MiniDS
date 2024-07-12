#pragma once

class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

	bool IsPlayer() { return _isPlayer; }

public:
	atomic<weak_ptr<class Room>> room;

public:
	Protocol::ObjectInfo* objectInfo;
	Protocol::PosInfo* posInfo;

protected:
	bool _isPlayer = false;
};
