#include "pch.h"
#include "Service.h"
#include "IocpCore.h"
#include "Session.h"
#include "Listener.h"

Service::Service(ServiceType type, NetAddress address, shared_ptr<IocpCore> iocpCore, SessionFactory factory, uint32 maxSessionCount)
	: _type(type), _netAddress(address), _sessionFactory(factory), _iocpCore(iocpCore), _maxSessionCount(maxSessionCount)
{
}

Service::~Service()
{

}

void Service::CloseService()
{

}

shared_ptr<Session> Service::CreateSession()
{
	shared_ptr<Session> session = _sessionFactory();
	session->SetService(shared_from_this());

	if (_iocpCore->Register(session) == false)
		return nullptr;

	return session;
}

void Service::AddSession(shared_ptr<Session> session)
{
	WRITE_LOCK;
	_sessionCount++;
	_sessions.insert(session);
	wcout << "[System] Current session count : " << GetCurrentSessionCount() << endl;
}

void Service::ReleaseSession(shared_ptr<Session> session)
{
	WRITE_LOCK;
	ASSERT_CRASH(_sessions.erase(session) != 0);
	_sessionCount--;
	wcout << "[System] Current session count : " << GetCurrentSessionCount() << endl;
}


ClientService::ClientService(NetAddress address, shared_ptr<IocpCore> iocpCore, SessionFactory factory, uint32 maxSessionCount)
	: Service(ServiceType::Client, address, iocpCore, factory, maxSessionCount)
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; i++)
	{
		shared_ptr<Session> session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}


ServerService::ServerService(NetAddress address, shared_ptr<IocpCore> iocpCore, SessionFactory factory, uint32 maxSessionCount)
	: Service(ServiceType::Server, address, iocpCore, factory, maxSessionCount)
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;

	_listener = make_shared<Listener>();
	if (_listener == nullptr)
		return false;

	shared_ptr<ServerService> service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false) // register iocp
		return false;

	return true;
}

void ServerService::CloseService()
{
	Service::CloseService();
}
