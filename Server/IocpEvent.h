#pragma once

class Session;
class IocpObject;

enum class EventType : uint8
{
	Connect,
	Accept,
	Recv,
	Send,
};


// 이 클래스와 이 클래스를 상속받는 클래스의 함수를 virtual로 선언하면 안됨.
// 이 클래스 주소 메모리 첫번째에 virtual table 주소가 들어가서
// OVERLAPPED 구조체를 사용할 수 없게 되기 때문.
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void Init();

	EventType GetType() { return _type; };
	void SetOwner(shared_ptr<IocpObject> owner) { _owner = owner; };
	shared_ptr<IocpObject> GetOwner() { return _owner; };

private:
	EventType _type;
	shared_ptr<IocpObject> _owner;
};


class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) {}
};


class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

	void SetSession(shared_ptr<Session> session) { _session = session; }
	shared_ptr<Session> GetSession() { return _session; }

private:
	shared_ptr<Session> _session = nullptr;
};


class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};


class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}
};
