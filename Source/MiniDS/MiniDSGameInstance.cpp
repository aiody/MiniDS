// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniDSGameInstance.h"
#include "Common/TcpSocketBuilder.h"
#include "PacketSession.h"
#include "Kismet/GameplayStatics.h"
#include "MiniDSCharacter.h"

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

void UMiniDSGameInstance::HandleSpawn(const Protocol::PlayerInfo& PlayerInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// 중복 처리 체크
	const uint64 Id = PlayerInfo.id();
	if (Players.Find(Id) != nullptr)
		return;

	FVector SpawnLocation(PlayerInfo.x(), PlayerInfo.y(), PlayerInfo.z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AMiniDSCharacter* Player = Cast<AMiniDSCharacter>(PC->GetPawn());
		if (Player == nullptr)
			return;

		MyPlayer = Player;
		Players.Add(PlayerInfo.id(), Player);
	}
	else
	{
		AMiniDSCharacter* Player = Cast<AMiniDSCharacter>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
		Players.Add(PlayerInfo.id(), Player);
	}
}

void UMiniDSGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UMiniDSGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
		HandleSpawn(Player, false);
}

void UMiniDSGameInstance::HandleDespawn(uint64 Id)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AMiniDSCharacter** Player = Players.Find(Id);
	if (Player == nullptr)
		return;

	World->DestroyActor(*Player);
}

void UMiniDSGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& Id : DespawnPkt.ids())
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
