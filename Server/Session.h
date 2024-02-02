#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

class Session : public IocpObject
{
	friend class Listener;

public:
	Session();
	virtual ~Session();

public:
	void				Send(BYTE* buffer, int32 len);
	void				Disconnect(const WCHAR* cause);

public:
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetNetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	bool				IsConnected() { return _connected; }
	shared_ptr<Session>	GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

public:
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { Send(buffer, len); return len; }
	virtual void		OnSend(int32 len) {}
	virtual void		OnDisconnected() {}

public:
	BYTE _recvBuffer[1024]; //TEMP

private:
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	atomic<bool>		_connected = false;

private:
	USE_LOCK;

private:
	RecvEvent			_recvEvent;
};
