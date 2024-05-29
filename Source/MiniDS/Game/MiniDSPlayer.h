// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "MiniDSPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MINIDS_API AMiniDSPlayer : public ACreature
{
	GENERATED_BODY()

public:
	AMiniDSPlayer();
	virtual ~AMiniDSPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void AnimNotify_Attack() override;
};
