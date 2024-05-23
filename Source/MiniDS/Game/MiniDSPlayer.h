// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "PaperZDCharacter.h"
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
UENUM(BlueprintType)
enum class ECreatureState : uint8
{
	E_CREATURE_STATE_IDLE = 0,
	E_CREATURE_STATE_MOVING = 1,
	E_CREATURE_STATE_ATTACK = 2,
	E_CREATURE_STATE_HIT = 3,
	E_CREATURE_STATE_DEATH = 4,
};

/**
 * 
 */
UCLASS()
class MINIDS_API AMiniDSPlayer : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	AMiniDSPlayer();
	virtual ~AMiniDSPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	bool IsMyPlayer();

	Protocol::MoveDir GetMoveDir() { return PlayerInfo->dir(); }
	void SetMoveDir(Protocol::MoveDir Dir);
	Protocol::CreatureState GetState() { return PlayerInfo->state(); }
	void SetState(Protocol::CreatureState State);
	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }
	void SetDestInfo(const Protocol::PlayerInfo& Info);

	void Hit(float Damage);

	// animation
	UFUNCTION(BlueprintCallable)
	FVector2D GetMoveDirVec2D();
	UFUNCTION(BlueprintCallable)
	ECreatureState GetStateWrapped();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_Attack();
	UFUNCTION(BlueprintCallable)
	void BackToIdle();

	// stat
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return StatInfo->hp(); };

public:
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookIdleDown;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookIdleSide;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookIdleUp;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunDown;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunSide;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunUp;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookAttackDown;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookAttackSide;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookAttackUp;
	UPROPERTY(EditAnywhere, Category = Battle)
	TSubclassOf<AActor> FloatingDamageClass;

protected:
	Protocol::PlayerInfo* PlayerInfo; // 현재 위치
	Protocol::PlayerInfo* DestInfo; // 목적지
	Protocol::StatInfo* StatInfo;
};
