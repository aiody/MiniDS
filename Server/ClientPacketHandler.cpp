#include "pch.h"
#include "ClientPacketHandler.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handler_C_CHAT(shared_ptr<PacketSession>& session, Protocol::C_CHAT& pkt)
{
	cout << "Handler_C_CHAT Message = " << pkt.msg() << endl;

	this_thread::sleep_for(1s);

	Protocol::S_CHAT pkt_chat;
	pkt_chat.set_msg(pkt.msg());
	shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt_chat);

	session->Send(sendBuffer);

	return true;
}
