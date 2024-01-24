#pragma once
#include "IocpCore.h"
#include "NetAddress.h"
#include "IocpEvent.h"

class Session : public IocpObject
{
	friend class Listener;

public:
	Session();
	~Session();

public:
	void Disconnect(const WCHAR* cause);
	

public:
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress GetNetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }
	bool IsConnected() { return _connected; }
	shared_ptr<Session> GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

public:
	HANDLE GetHandle() override;
	void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	void RegisterConnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errorCode);

protected:
	virtual void OnConnected() {}
	virtual void OnRecv() {}
	virtual void OnSend() {}
	virtual void OnDisconnected() {}

public:
	BYTE _recvBuffer[1024];

private:
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddress = {};
	atomic<bool> _connected = false;
	RecvEvent _recvEvent;
};
