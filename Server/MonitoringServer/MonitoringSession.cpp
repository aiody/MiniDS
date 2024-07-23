#include "pch.h"
#include "MonitoringSession.h"
#include "MonitoringPacketHandler.h"

MonitoringSession::MonitoringSession()
{
}

MonitoringSession::~MonitoringSession()
{
}

void MonitoringSession::OnConnected()
{
	shared_ptr<PacketSession> session = GetPacketSessionRef();
	
	Protocol::M_REQ_SERVER_INFO reqServerInfoPkt;
	shared_ptr<SendBuffer> sendBuffer = MonitoringPacketHandler::MakeSendBuffer(reqServerInfoPkt);
	session->Send(sendBuffer);
}

void MonitoringSession::OnDisconnected()
{
}

void MonitoringSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	shared_ptr<PacketSession> session = GetPacketSessionRef();
	MonitoringPacketHandler::HandlePacket(session, buffer, len);
}

void MonitoringSession::OnSend(int32 len)
{
}
