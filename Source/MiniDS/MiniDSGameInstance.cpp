// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniDSGameInstance.h"
#include "Common/TcpSocketBuilder.h"
#include "PacketSession.h"
#include "Kismet/GameplayStatics.h"
#include "MiniDSPlayer.h"
#include "Pig.h"

void UMiniDSGameInstance::ConnectToGameServer()
{
	if (Socket == nullptr)
	{
		Socket = FTcpSocketBuilder(TEXT("Client Socket"))
			.AsBlocking()
			.AsReusable()
			.BoundToEndpoint(FIPv4Endpoint::Any);

		FIPv4Address Ip;
		FIPv4Address::Parse(IpAddress, Ip);
		FIPv4Endpoint endPoint(Ip, Port);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

		bool Connected = Socket->Connect(endPoint.ToInternetAddr().Get());
		if (Connected)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));
			 
			GameServerSession = MakeShared<PacketSession>(Socket);
			GameServerSession->Run();

			// 게임 접속 패킷 전송
			{
				Protocol::C_ENTER_GAME Pkt;
				SEND_PACKET(Pkt);
			}
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UMiniDSGameInstance::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME Pkt;
	SEND_PACKET(Pkt);
}

void UMiniDSGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UMiniDSGameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UMiniDSGameInstance::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// 중복 처리 체크
	const uint64 Id = ObjectInfo.object_id();
	if (Players.Find(Id) != nullptr)
		return;

	Protocol::PosInfo PosInfo = ObjectInfo.pos_info();
	FVector SpawnLocation(PosInfo.x(), PosInfo.y(), PosInfo.z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AMiniDSPlayer* Player = Cast<AMiniDSPlayer>(PC->GetPawn());
		if (Player == nullptr)
			return;

		MyPlayer = Player;
		Player->SetObjectInfo(ObjectInfo);
		Player->SetPosInfo(PosInfo);
		Players.Add(Id, Player);
	}
	else
	{
		if (ObjectInfo.has_creature_info() &&
			ObjectInfo.creature_info().creature_type() == Protocol::CREATURE_TYPE_MONSTER)
		{
			// 몬스터 스폰
			APig* Pig = Cast<APig>(World->SpawnActor(PigClass, &SpawnLocation));
			Pig->SetObjectInfo(ObjectInfo);
			Monsters.Add(Id, Pig);
		}
		else
		{
			AMiniDSPlayer* Player = Cast<AMiniDSPlayer>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
			Player->SetObjectInfo(ObjectInfo);
			Players.Add(Id, Player);
		}
	}
}

void UMiniDSGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UMiniDSGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Object : SpawnPkt.objects())
		HandleSpawn(Object, false);
}

void UMiniDSGameInstance::HandleDespawn(uint64 Id)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AMiniDSPlayer** FoundActor = Players.Find(Id);
	if (FoundActor == nullptr)
		return;

	World->DestroyActor(*FoundActor);
}

void UMiniDSGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& Id : DespawnPkt.object_ids())
		HandleDespawn(Id);
}

void UMiniDSGameInstance::HandleLeaveGame()
{
	GameServerSession->Disconnect();

	if (Socket)
	{
		ISocketSubsystem::Get()->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void UMiniDSGameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 Id = MovePkt.object_id();

	AMiniDSPlayer** FoundActor = Players.Find(Id);
	if (FoundActor == nullptr)
		return;

	AMiniDSPlayer* Player = (*FoundActor);
	if (Player->IsMyPlayer())
		return;
	
	const Protocol::PosInfo& PosInfo = MovePkt.pos_info();
	Player->SetDestInfo(PosInfo);
	Player->SetState(MovePkt.state());
}

void UMiniDSGameInstance::HandleHit(const Protocol::S_HIT& HitPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 Id = HitPkt.to();
	const float Damage = HitPkt.damage();

	AMiniDSPlayer** FoundActor = Players.Find(Id);
	if (FoundActor == nullptr)
		return;

	AMiniDSPlayer* Player = (*FoundActor);
	Player->SetState(Protocol::CREATURE_STATE_HIT);
	Player->Hit(Damage);
}

void UMiniDSGameInstance::HandleDeath(const Protocol::S_DEATH& DeathPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 Id = DeathPkt.to();
	const float Damage = DeathPkt.damage();

	AMiniDSPlayer** FoundActor = Players.Find(Id);
	if (FoundActor == nullptr)
		return;

	AMiniDSPlayer* Player = (*FoundActor);
	Player->SetState(Protocol::CREATURE_STATE_DEATH);
	Player->Hit(Damage);
}
