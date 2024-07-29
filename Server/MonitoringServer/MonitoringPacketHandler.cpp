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
	cout << "Num of session : " << pkt.game_session_num() << endl;
	cout << "Num of thread : " << pkt.thread_num() << endl;
	cout << "Total virtual memory : " << pkt.total_virtual_memory() << endl;
	cout << "Virtual memory currently used : " << pkt.virtual_memory_currently_used() << endl;
	cout << "Virtual memory used by me : " << pkt.virtual_memory_used_by_me() << endl;
	cout << "Total physical memory : " << pkt.total_physical_memory() << endl;
	cout << "Physical memory currently used : " << pkt.physical_memory_currently_used() << endl;
	cout << "Physical memory used by me : " << pkt.physical_memory_used_by_me() << endl;
	cout << "CPU currently used : " << pkt.cpu_currently_used() << endl;
	cout << "CPU used by me : " << pkt.cpu_used_by_me() << endl << endl;
	

	this_thread::sleep_for(1s);
	
	// 다시 요청
	Protocol::M_REQ_SERVER_INFO reqServerInfoPkt;
	shared_ptr<SendBuffer> sendBuffer = MonitoringPacketHandler::MakeSendBuffer(reqServerInfoPkt);
	session->Send(sendBuffer);

	return true;
}
