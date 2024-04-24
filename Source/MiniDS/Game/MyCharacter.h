// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/MiniDSCharacter.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MINIDS_API AMyCharacter : public AMiniDSCharacter
{
	GENERATED_BODY()
	
public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	void Move(const FInputActionValue& Value);
	void ReleaseMove(const FInputActionValue& Value);
	void UpdateAnimation(const FVector2D MovementVector);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

public:
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookIdle;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunSide;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunUp;
	UPROPERTY(EditAnywhere, Category = Animation)
	UPaperFlipbook* FlipbookRunDown;
};
