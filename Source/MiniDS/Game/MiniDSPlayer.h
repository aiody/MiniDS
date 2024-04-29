// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Protocol.pb.h"
#include "MiniDSPlayer.generated.h"

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
class MINIDS_API AMiniDSPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	AMiniDSPlayer();
	virtual ~AMiniDSPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void UpdateAnimation(const FVector2D MovementVector);
	void UpdateAnimation(Protocol::MoveState State);

public:
	bool IsMyPlayer();

	Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }
	void SetMoveState(Protocol::MoveState State);
	
	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }
	
	void SetDestInfo(const Protocol::PlayerInfo& Info);

public:
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookIdle;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunSide;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunUp;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunDown;

protected:
	Protocol::PlayerInfo* PlayerInfo; // 현재 위치
	Protocol::PlayerInfo* DestInfo; // 목적지
};
