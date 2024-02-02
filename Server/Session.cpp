#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "IocpEvent.h"

Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::CloseSocket(_socket);
}

void Session::Send(BYTE* buffer, int32 len)
{
	SendEvent* sendEvent = new SendEvent();
	sendEvent->SetOwner(shared_from_this()); // ADD_REF
	sendEvent->buffer.resize(len);
	::memcpy(sendEvent->buffer.data(), buffer, len);

	WRITE_LOCK
	RegisterSend(sendEvent);
}

void Session::Disconnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
		return;

	wcout << "Disconnect : " << cause << endl;

	OnDisconnected();

	SocketUtils::CloseSocket(_socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->GetType())
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numOfBytes);
		break;
	default:
		break;
	}
}

void Session::RegisterConnect()
{
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	_recvEvent.Init();
	_recvEvent.SetOwner(shared_from_this()); // ADD_REF

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer);
	wsaBuf.len = len32(_recvBuffer);
	DWORD numOfBytes = 0;
	DWORD flag = 0;

	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT &numOfBytes, &flag, &_recvEvent, NULL))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_recvEvent.SetOwner(nullptr); // RELEASE_REF
		}
	}
}

void Session::RegisterSend(SendEvent* sendEvent)
{
	if (IsConnected() == false)
		return;

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(sendEvent->buffer.data());
	wsaBuf.len = sendEvent->buffer.size();

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, OUT &numOfBytes, 0, sendEvent, NULL))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			sendEvent->SetOwner(nullptr); // RELEASE_REF
			delete sendEvent;
		}
	}
}

void Session::ProcessConnect()
{
	_connected.store(true);
	
	OnConnected();
	
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.SetOwner(nullptr); // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	cout << "Recv Data Len = " << numOfBytes << endl;

	OnRecv(_recvBuffer, numOfBytes);

	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numOfBytes)
{
	sendEvent->SetOwner(nullptr); // RELEASE_REF
	delete sendEvent;

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	cout << "Send Data Len = " << numOfBytes << endl;

	OnSend(numOfBytes);
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}
