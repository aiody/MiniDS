// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Creature.h"
#include "Pig.generated.h"

/**
 * 
 */
UCLASS()
class MINIDS_API APig : public ACreature
{
	GENERATED_BODY()

public:
	APig();
	virtual ~APig();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void AnimNotify_Attack() override;
};
