#pragma once

class DummyClientSession : public PacketSession
{
public:
	DummyClientSession();
	virtual ~DummyClientSession();

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	void SetPlayerInfo(const Protocol::ObjectInfo& playerInfo) { _playerInfo->MergeFrom(playerInfo); }

private:
	shared_ptr<DummyClientSession> GetDummyClientSessionRef() { return static_pointer_cast<DummyClientSession>(GetPacketSessionRef()); }
	void UpdateTick();
	void SendAttackPacketIntermittently();

private:
	Protocol::ObjectInfo* _playerInfo;
};
