// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniDSGameInstance.h"
#include "Common/TcpSocketBuilder.h"
#include "PacketSession.h"

#include "PaperCharacter.h"

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
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UMiniDSGameInstance::DisconnectFromGameServer()
{
	if (Socket)
	{
		ISocketSubsystem::Get()->DestroySocket(Socket);
		Socket = nullptr;
	}
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

void UMiniDSGameInstance::SpawnWeber()
{
	FVector SpawnLocation(0, 0, 100);
	FRotator SpawnRotation(0, 0, 0);
	
	auto* World = GetWorld();
	if (World == nullptr)
		return;
	
	//World->SpawnActor<APaperCharacter>(Weber, SpawnLocation, SpawnRotation);
	World->SpawnActor(Weber, &SpawnLocation);
}
