#include "pch.h"
#include "ServerPacketHandler.h"
#include "Room.h"
#include "JobQueue.h"
#include "Protocol.pb.h"
#include "MonitoringInfo.h"

GameSession::GameSession()
{
}

GameSession::~GameSession()
{
}

void GameSession::OnConnected()
{
}

void GameSession::OnDisconnected()
{
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
    PacketHeader header = *(reinterpret_cast<PacketHeader*>(buffer));
    //cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

    shared_ptr<PacketSession> session = GetPacketSessionRef();
    if (ServerPacketHandler::HandlePacket(session, buffer, len))
        gMonitoringInfo->AddTransferredPacketCount();
}

void GameSession::OnSend(int32 len)
{
    //cout << "OnSend Len = " << len << endl;
}
