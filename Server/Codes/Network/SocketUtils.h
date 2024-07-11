#pragma once

class SocketUtils
{
public:
	static LPFN_ACCEPTEX		AcceptEx;
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;

public:
	static void		Init();
	static void		Clear();

	static bool		BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);

	// 소켓 생성/삭제
	static SOCKET	CreateSocket();
	static void		CloseSocket(SOCKET socket);

	// 소켓 관련 함수들
	static bool		Bind(SOCKET socket, class NetAddress netAddr);
	static bool		BindAnyAddress(SOCKET socket, uint16 port);
	static bool		Listen(SOCKET socket, int32 backlog = SOMAXCONN);

	// 소켓 옵션 설정 함수들
	static bool		SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool		SetReuseAddress(SOCKET socket, bool reuse);
	static bool		SetRecvBufferSize(SOCKET socket, int32 size);
	static bool		SetSendBufferSize(SOCKET socket, int32 size);
	static bool		SetTcpNoDelay(SOCKET socket, bool noDelay);
	static bool		SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	template<typename T>
	static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
	{
		return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<const char*>(&optVal), sizeof(T));
	}
};
