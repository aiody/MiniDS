#include "pch.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"

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
    cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

    char recvBuffer[4096] = { 0 };
    ::memcpy(recvBuffer, &buffer[4], header.size - sizeof(PacketHeader));
    cout << "OnRecvPacket Message = " << recvBuffer << ", Len = " << header.size << endl;

    shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
    sendBuffer->CopyData(recvBuffer, header.size - sizeof(PacketHeader));

    for (int i = 0; i < 5; i++)
        Send(sendBuffer);

    this_thread::sleep_for(1s);

    //shared_ptr<PacketSession> session = GetPacketSessionRef();
    //ClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
    cout << "OnSend Len = " << len << endl;
}
