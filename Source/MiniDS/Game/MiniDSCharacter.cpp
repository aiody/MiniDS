// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniDSCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyCharacter.h"

AMiniDSCharacter::AMiniDSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(88.f, 155.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	PlayerInfo = new Protocol::PlayerInfo();
}

AMiniDSCharacter::~AMiniDSCharacter()
{
	delete PlayerInfo;
	PlayerInfo = nullptr;
}

void AMiniDSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMiniDSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	{
		FVector Location = GetActorLocation();
		PlayerInfo->set_x(Location.X);
		PlayerInfo->set_y(Location.Y);
		PlayerInfo->set_z(Location.Z);
	}
}

bool AMiniDSCharacter::IsMyPlayer()
{
	return Cast<AMyCharacter>(this) != nullptr;
}

void AMiniDSCharacter::SetPlayerInfo(const Protocol::PlayerInfo& Info)
{
	PlayerInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}
