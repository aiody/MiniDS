// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
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

protected:
	virtual void BeginPlay() override;
};
