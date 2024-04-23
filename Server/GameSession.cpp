#include "pch.h"
#include "ServerPacketHandler.h"
#include "Room.h"
#include "JobQueue.h"
#include "Protocol.pb.h"

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
    //shared_ptr<GameSession> session = static_pointer_cast<GameSession>(GetPacketSessionRef());
    //gJobQueue->Push(make_shared<Job>(gRoom, &Room::LeavePlayer, session->curPlayer));
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
    PacketHeader header = *(reinterpret_cast<PacketHeader*>(buffer));
    cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

    shared_ptr<PacketSession> session = GetPacketSessionRef();
    ServerPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
    cout << "OnSend Len = " << len << endl;
}
