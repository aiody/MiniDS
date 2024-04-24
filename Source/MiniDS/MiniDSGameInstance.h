// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MiniDS.h"
#include "Protocol/Protocol.pb.h"
#include "MiniDSGameInstance.generated.h"

class AMiniDSCharacter;

/**
 * 
 */
UCLASS()
class MINIDS_API UMiniDSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

public:
	void HandleSpawn(const Protocol::PlayerInfo& PlayerInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);
	void HandleDespawn(uint64 Id);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);
	void HandleLeaveGame();

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 9999;

	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMiniDSCharacter> OtherPlayerClass;

	AMiniDSCharacter* MyPlayer;
	TMap<uint64, AMiniDSCharacter*> Players;
};
