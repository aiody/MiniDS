// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Pig.h"
#include "Components/CapsuleComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APig::APig()
{
	GetCapsuleComponent()->InitCapsuleSize(85.f, 160.f);
	GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, -160.f));
	GetSprite()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
}

APig::~APig()
{
}

void APig::BeginPlay()
{
	Super::BeginPlay();
}

void APig::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APig::AnimNotify_Attack()
{
	Super::AnimNotify_Attack();
}
