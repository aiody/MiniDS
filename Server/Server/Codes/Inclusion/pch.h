#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용하지 않는 API를 제외하여 Win32 헤더 파일의 크기를 줄입니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"

// Contents
#include "Utils.h"
#include "Protocol.pb.h"
#include "GameSession.h"

#define SEND_PACKET(pkt)															\
	shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);	\
	session->Send(sendBuffer);
