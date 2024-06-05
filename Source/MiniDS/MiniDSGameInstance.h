// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MiniDS.h"
#include "Protocol/Protocol.pb.h"
#include "MiniDSGameInstance.generated.h"

class ACreature;
class AMiniDSPlayer;
class APig;

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
	void HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);
	void HandleDespawn(uint64 Id);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);
	void HandleLeaveGame();
	void HandleMove(const Protocol::S_MOVE& MovePkt);
	void HandleHit(const Protocol::S_HIT& HitPkt);
	void HandleDeath(const Protocol::S_DEATH& DeathPkt);

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 9999;

	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMiniDSPlayer> OtherPlayerClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APig> PigClass;

	AMiniDSPlayer* MyPlayer;
	TMap<uint64, ACreature*> Creatures;
};
