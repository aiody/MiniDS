#include "pch.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"
#include "Room.h"
#include "JobQueue.h"
#include "Player.h"
#include "Protocol.pb.h"

GameSession::GameSession()
{
}

GameSession::~GameSession()
{
}

void GameSession::OnConnected()
{
    shared_ptr<GameSession> session = static_pointer_cast<GameSession>(GetPacketSessionRef());

    shared_ptr<Player> player = make_shared<Player>();
    player->ownerSession = session;
    player->playerId = GenPlayerId();
    session->curPlayer = player;
    gJobQueue->Push(make_shared<Job>(gRoom, &Room::Enter, player));

    Protocol::S_EnterGame pktEnterGame;
    pktEnterGame.set_playerid(player->playerId);
    shared_ptr<SendBuffer> enterGameBuffer = ClientPacketHandler::MakeSendBuffer(pktEnterGame);
    GetPacketSessionRef()->Send(enterGameBuffer);

    Protocol::S_Spawn pktSpawn;
    pktSpawn.mutable_player()->set_id(player->playerId);
    shared_ptr<SendBuffer> spawnBuffer = ClientPacketHandler::MakeSendBuffer(pktSpawn);
    gJobQueue->Push(make_shared<Job>(gRoom, &Room::Broadcast, spawnBuffer));
}

void GameSession::OnDisconnected()
{
    shared_ptr<GameSession> session = static_pointer_cast<GameSession>(GetPacketSessionRef());
    gJobQueue->Push(make_shared<Job>(gRoom, &Room::Leave, session->curPlayer));
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
    PacketHeader header = *(reinterpret_cast<PacketHeader*>(buffer));
    cout << "Packet ID : " << header.id << " Size : " << header.size << endl;

    shared_ptr<PacketSession> session = GetPacketSessionRef();
    ClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
    cout << "OnSend Len = " << len << endl;
}

uint64 GameSession::GenPlayerId()
{
    static uint64 id = 1;
    return id++;
}
