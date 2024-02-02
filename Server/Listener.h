#pragma once
#include "IocpCore.h"

class AcceptEvent;

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	bool StartAccept(class NetAddress netAddress);
	void CloseSocket();

public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
};
