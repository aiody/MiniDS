#include "pch.h"
#include "MonitoringSession.h"
#include "MonitoringPacketHandler.h"
#include "JobTimer.h"
#include "MonitoringService.h"

MonitoringSession::MonitoringSession()
{
}

MonitoringSession::~MonitoringSession()
{
}

void MonitoringSession::OnConnected()
{
	gMonitoringService->SetServerRunningStatus(true);

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

void MonitoringSession::SetMonitoringInfo(Protocol::S_RES_SERVER_INFO& pkt)
{
	gMonitoringService->SetLastRespondTime();
	gMonitoringService->SetMonitoringInfo(
		pkt.cpu_used_by_me(),
		pkt.virtual_memory_used_by_me() / 1000000,
		pkt.game_session_num(),
		pkt.thread_num(),
		pkt.server_start_time(),
		pkt.packet_transfers_per_second());
}
