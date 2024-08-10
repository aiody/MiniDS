#include "pch.h"
#include "DummyClientPacketHandler.h"
#include "DummyClientSession.h"

PacketHandleFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
    return false;
}

bool Handler_S_ENTER_GAME(shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& pkt)
{
    shared_ptr<DummyClientSession> clientSession = static_pointer_cast<DummyClientSession>(session);
    clientSession->SetPlayerInfo(pkt.player());
    return true;
}

bool Handler_S_LEAVE_GAME(shared_ptr<PacketSession>& session, Protocol::S_LEAVE_GAME& pkt)
{
    return false;
}

bool Handler_S_SPAWN(shared_ptr<PacketSession>& session, Protocol::S_SPAWN& pkt)
{
    return false;
}

bool Handler_S_DESPAWN(shared_ptr<PacketSession>& session, Protocol::S_DESPAWN& pkt)
{
    return false;
}

bool Handler_S_MOVE(shared_ptr<PacketSession>& session, Protocol::S_MOVE& pkt)
{
    return false;
}

bool Handler_S_HIT(shared_ptr<PacketSession>& session, Protocol::S_HIT& pkt)
{
    return false;
}

bool Handler_S_DEATH(shared_ptr<PacketSession>& session, Protocol::S_DEATH& pkt)
{
    return false;
}
