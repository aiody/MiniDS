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

	Protocol::MoveDir		GetMoveDir() { return PosInfo->dir(); }
	void					SetMoveDir(Protocol::MoveDir Dir);
	Protocol::CreatureState GetState() { return CreatureInfo->state(); }
	void					SetState(Protocol::CreatureState State);
	Protocol::ObjectInfo*	GetObjectInfo() { return ObjectInfo; }
	void					SetObjectInfo(const Protocol::ObjectInfo& Info);
	Protocol::CreatureInfo*	GetCreatureInfo() { return CreatureInfo; }
	Protocol::PosInfo*		GetPosInfo() { return PosInfo; }
	void					SetPosInfo(const Protocol::PosInfo& Info);
	void					SetDestInfo(const Protocol::PosInfo& Info);

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
	float GetHealth() { return CreatureInfo->stat_info().hp(); };

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
	Protocol::ObjectInfo* ObjectInfo;
	Protocol::CreatureInfo* CreatureInfo;
	Protocol::StatInfo* StatInfo;
	Protocol::PosInfo* PosInfo; // 현재 위치
	Protocol::PosInfo* DestInfo; // 목적지
};
