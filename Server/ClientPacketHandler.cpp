#include "pch.h"
#include "ClientPacketHandler.h"
#include "Job.h"
#include "JobQueue.h"
#include "JobTimer.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handler_C_CHAT(shared_ptr<PacketSession>& session, Protocol::C_CHAT& pkt)
{
	shared_ptr<Job> job = make_shared<Job>([session, pkt]() {
		cout << "ChatJob = " << pkt.msg() << endl;

		this_thread::sleep_for(1s);

		Protocol::S_CHAT pkt_chat;
		pkt_chat.set_msg(pkt.msg());
		shared_ptr<SendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt_chat);
		session->Send(sendBuffer);
		});

	//gJobQueue->Push(job);
	gJobTimer->Reserve(3000, job);

	return true;
}
