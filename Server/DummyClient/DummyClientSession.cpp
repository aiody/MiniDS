#include "pch.h"
#include "DummyClientSession.h"
#include "DummyClientPacketHandler.h"
#include "Utils.h"

DummyClientSession::DummyClientSession()
{
	_playerInfo = new Protocol::ObjectInfo();
}

DummyClientSession::~DummyClientSession()
{
	delete _playerInfo;
}

void DummyClientSession::OnConnected()
{
	Protocol::C_ENTER_GAME enterGamePkt;
	shared_ptr<SendBuffer> sendBuffer = DummyClientPacketHandler::MakeSendBuffer(enterGamePkt);
	Send(sendBuffer);

	UpdateTick();
	gJobTimer->Reserve(250, make_shared<Job>(GetDummyClientSessionRef(), &DummyClientSession::SendAttackPacketIntermittently));
}

void DummyClientSession::OnDisconnected()
{
}

void DummyClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	shared_ptr<PacketSession> session = GetPacketSessionRef();
	DummyClientPacketHandler::HandlePacket(session, buffer, len);
}

void DummyClientSession::OnSend(int32 len)
{
}

void DummyClientSession::UpdateTick()
{
	if (_playerInfo == nullptr) return;

	Protocol::C_MOVE movePkt;
	{
		movePkt.set_object_id(_playerInfo->object_id());
		Protocol::PosInfo* info = movePkt.mutable_pos_info();
		info->CopyFrom(_playerInfo->pos_info());
		movePkt.set_state(_playerInfo->creature_info().state());
	}
	shared_ptr<SendBuffer> sendBuffer = DummyClientPacketHandler::MakeSendBuffer(movePkt);
	Send(sendBuffer);

	gJobTimer->Reserve(200, make_shared<Job>(GetDummyClientSessionRef(), &DummyClientSession::UpdateTick));
}

void DummyClientSession::SendAttackPacketIntermittently()
{
	if (_playerInfo == nullptr) return;

	Protocol::C_ATTACK attackPkt;
	{
		attackPkt.set_from(_playerInfo->object_id());
		attackPkt.set_to(Utils::GetRandom(1, 1000));
	}
	shared_ptr<SendBuffer> sendBuffer = DummyClientPacketHandler::MakeSendBuffer(attackPkt);
	Send(sendBuffer);

	uint32 randomTime = Utils::GetRandom(500, 5000);
	gJobTimer->Reserve(randomTime, make_shared<Job>(GetDummyClientSessionRef(), &DummyClientSession::SendAttackPacketIntermittently));
}
