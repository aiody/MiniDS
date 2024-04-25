// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Protocol.pb.h"
#include "MiniDSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class MINIDS_API AMiniDSCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	AMiniDSCharacter();
	virtual ~AMiniDSCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	bool IsMyPlayer();

public:
	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }

protected:
	Protocol::PlayerInfo* PlayerInfo; // 현재 위치
};
