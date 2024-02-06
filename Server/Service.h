#pragma once
#include "NetAddress.h"
#include <functional>

class IocpCore;
class Session;
class Listener;
enum class ServiceType : uint8
{
	Client,
	Server,
};

using SessionFactory = function<shared_ptr<Session>()>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address, shared_ptr<IocpCore> iocpCore, SessionFactory factory, uint32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool				Start() abstract;
	bool						CanStart() { return _sessionFactory != nullptr; }
	virtual void				CloseService();

	ServiceType					GetServiceType() { return _type; }
	NetAddress					GetNetAddress() { return _netAddress; }
	shared_ptr<IocpCore>		GetIocpCore() { return _iocpCore; }
	int32						GetMaxSessionCount() { return _maxSessionCount; }

	shared_ptr<Session>			CreateSession();
	void						AddSession(shared_ptr<Session> session);
	void						ReleaseSession(shared_ptr<Session> session);
	int32						GetCurrentSessionCount() { return _sessionCount; }

protected:
	USE_LOCK;
	ServiceType					_type;
	NetAddress					_netAddress = {};
	shared_ptr<IocpCore>		_iocpCore;

	set<shared_ptr<Session>>	_sessions;
	int32						_sessionCount = 0;
	int32						_maxSessionCount = 0;
	SessionFactory				_sessionFactory;
};

class ClientService : public Service
{
public:
	ClientService(NetAddress address, shared_ptr<IocpCore> iocpCore, SessionFactory factory, uint32 maxSessionCount);
	virtual ~ClientService() {};

	virtual bool Start() override;
};

class ServerService : public Service
{
public:
	ServerService(NetAddress address, shared_ptr<IocpCore> iocpCore, SessionFactory factory, uint32 maxSessionCount);
	virtual ~ServerService() {};

	virtual bool Start() override;
	virtual void CloseService() override;

private:
	shared_ptr<Listener> _listener = nullptr;
};
