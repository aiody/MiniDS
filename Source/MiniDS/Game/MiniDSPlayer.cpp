// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniDSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyMiniDSPlayer.h"
#include "PaperFlipbookComponent.h"

AMiniDSPlayer::AMiniDSPlayer()
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
	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	PlayerInfo = new Protocol::PlayerInfo();
	DestInfo = new Protocol::PlayerInfo();
}

AMiniDSPlayer::~AMiniDSPlayer()
{
	delete PlayerInfo;
	PlayerInfo = nullptr;
	delete DestInfo;
	DestInfo = nullptr;
}

void AMiniDSPlayer::BeginPlay()
{
	Super::BeginPlay();

	{
		// DestInfo 초기화
		FVector Location = GetActorLocation();
		DestInfo->set_x(Location.X);
		DestInfo->set_y(Location.Y);
		DestInfo->set_z(Location.Z);

		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
}

void AMiniDSPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	{
		FVector Location = GetActorLocation();
		PlayerInfo->set_x(Location.X);
		PlayerInfo->set_y(Location.Y);
		PlayerInfo->set_z(Location.Z);
	}

	if (IsMyPlayer() == false)
	{
		const Protocol::MoveState State = PlayerInfo->state();

		FVector Location = GetActorLocation();
		FVector DestLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		FVector MoveDir = (DestLocation - Location);
		const float DistToDest = MoveDir.Length();
		MoveDir.Normalize();
		
		if (State == Protocol::MOVE_STATE_IDLE)
		{
			if (DistToDest > 10.f)
				AddMovementInput(MoveDir);
			else
				UpdateAnimation(FVector2D::Zero());
		}
		else
		{
			AddMovementInput(MoveDir);
			UpdateAnimation(State);
		}
	}
}

void AMiniDSPlayer::UpdateAnimation(const FVector2D MovementVector)
{
	if (MovementVector.X == 0 && MovementVector.Y == 0)
	{
		GetSprite()->SetFlipbook(FlipbookIdle);
		return;
	}

	if (MovementVector.X > 0)
	{
		GetSprite()->SetRelativeRotation(FRotator(0, 90.f, 0));
		GetSprite()->SetFlipbook(FlipbookRunSide);
	}
	else if (MovementVector.X < 0)
	{
		GetSprite()->SetRelativeRotation(FRotator(0, -90.f, 0));
		GetSprite()->SetFlipbook(FlipbookRunSide);
	}
	else
	{
		if (MovementVector.Y > 0)
		{
			GetSprite()->SetFlipbook(FlipbookRunUp);
		}
		else
		{
			GetSprite()->SetFlipbook(FlipbookRunDown);
		}
	}
}

void AMiniDSPlayer::UpdateAnimation(Protocol::MoveState State)
{
	switch (State)
	{
	case Protocol::MOVE_STATE_RUN_UP:
		UpdateAnimation(FVector2D(0, 1));
		break;
	case Protocol::MOVE_STATE_RUN_DOWN:
		UpdateAnimation(FVector2D(0, -1));
		break;
	case Protocol::MOVE_STATE_RUN_LEFT:
		UpdateAnimation(FVector2D(-1, 0));
		break;
	case Protocol::MOVE_STATE_RUN_RIGHT:
		UpdateAnimation(FVector2D(1, 0));
		break;
	case Protocol::MOVE_STATE_IDLE:
		UpdateAnimation(FVector2D(0, 0));
		break;
	default:
		UpdateAnimation(FVector2D(0, 0));
		break;
	}
}

bool AMiniDSPlayer::IsMyPlayer()
{
	return Cast<AMyMiniDSPlayer>(this) != nullptr;
}

void AMiniDSPlayer::SetMoveState(Protocol::MoveState State)
{
	if (PlayerInfo->state() == State)
		return;

	PlayerInfo->set_state(State);
}

void AMiniDSPlayer::SetPlayerInfo(const Protocol::PlayerInfo& Info)
{
	PlayerInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void AMiniDSPlayer::SetDestInfo(const Protocol::PlayerInfo& Info)
{
	DestInfo->CopyFrom(Info);

	SetMoveState(Info.state());
}
