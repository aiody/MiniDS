// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MiniDSGameInstance.generated.h"

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

	UFUNCTION(BlueprintCallable)
	void SpawnWeber();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Weber;

protected:
	class ClientSocket2* Socket2;
	class DispatcherThread* Thread;

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 9999;

	TSharedPtr<class PacketSession> GameServerSession;
};
