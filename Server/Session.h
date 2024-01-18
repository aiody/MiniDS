#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class Session : public IocpObject
{
public:
	Session();
	~Session();

public:
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress GetNetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }

public:
	HANDLE GetHandle() override;
	void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

public:
	BYTE _recvBuffer[1024];

private:
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddress = {};
};
