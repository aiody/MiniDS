#include "pch.h"
#include "MonitoringPacketHandler.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handler_S_RES_SERVER_INFO(shared_ptr<PacketSession>& session, Protocol::S_RES_SERVER_INFO& pkt)
{
	shared_ptr<MonitoringSession> monitoringSession = static_pointer_cast<MonitoringSession>(session);
	monitoringSession->SetMonitoringInfo(pkt);

	this_thread::sleep_for(1s);
	
	// 다시 요청
	Protocol::M_REQ_SERVER_INFO reqServerInfoPkt;
	shared_ptr<SendBuffer> sendBuffer = MonitoringPacketHandler::MakeSendBuffer(reqServerInfoPkt);
	session->Send(sendBuffer);

	return true;
}
