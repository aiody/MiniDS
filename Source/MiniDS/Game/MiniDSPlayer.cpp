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

		SetState(Protocol::CREATURE_STATE_IDLE);
		SetMoveDir(Protocol::MOVE_DIR_NONE);
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
		const Protocol::CreatureState State = GetState();

		FVector Location = GetActorLocation();
		FVector DestLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		FVector MoveDir = (DestLocation - Location);
		const float DistToDest = MoveDir.Length();
		MoveDir.Normalize();
		
		if (State == Protocol::CREATURE_STATE_IDLE)
		{
			if (DistToDest > 10.f)
			{
				AddMovementInput(MoveDir);
				UpdateAnimation(Protocol::CREATURE_STATE_MOVING);
			}
			else
				UpdateAnimation(Protocol::CREATURE_STATE_IDLE);
		}
		else
		{
			AddMovementInput(MoveDir);
			UpdateAnimation();
		}
	}
	else
	{
		UpdateAnimation();
	}
}

void AMiniDSPlayer::UpdateAnimation()
{
	Protocol::CreatureState State = GetState();
	UpdateAnimation(State);
}

void AMiniDSPlayer::UpdateAnimation(Protocol::CreatureState State)
{
	Protocol::MoveDir MoveDir = GetMoveDir();

	if (MoveDir == Protocol::MOVE_DIR_LEFT)
		GetSprite()->SetRelativeRotation(FRotator(0, -90.f, 0));
	else if (MoveDir == Protocol::MOVE_DIR_RIGHT)
		GetSprite()->SetRelativeRotation(FRotator(0, 90.f, 0));

	switch (State)
	{
	case Protocol::CREATURE_STATE_IDLE:
		switch (MoveDir)
		{
		case Protocol::MOVE_DIR_UP:
			GetSprite()->SetFlipbook(FlipbookIdleUp);
			break;
		case Protocol::MOVE_DIR_NONE:
		case Protocol::MOVE_DIR_DOWN:
			GetSprite()->SetFlipbook(FlipbookIdleDown);
			break;
		case Protocol::MOVE_DIR_LEFT:
		case Protocol::MOVE_DIR_RIGHT:
			GetSprite()->SetFlipbook(FlipbookIdleSide);
			break;
		}
		break;
	case Protocol::CREATURE_STATE_MOVING:
		switch (MoveDir)
		{
		case Protocol::MOVE_DIR_UP:
			GetSprite()->SetFlipbook(FlipbookRunUp);
			break;
		case Protocol::MOVE_DIR_NONE:
		case Protocol::MOVE_DIR_DOWN:
			GetSprite()->SetFlipbook(FlipbookRunDown);
			break;
		case Protocol::MOVE_DIR_LEFT:
		case Protocol::MOVE_DIR_RIGHT:
			GetSprite()->SetFlipbook(FlipbookRunSide);
			break;
		}
		break;
	case Protocol::CREATURE_STATE_ATTACK:
		switch (MoveDir)
		{
		case Protocol::MOVE_DIR_UP:
			GetSprite()->SetFlipbook(FlipbookAttackUp);
			break;
		case Protocol::MOVE_DIR_NONE:
		case Protocol::MOVE_DIR_DOWN:
			GetSprite()->SetFlipbook(FlipbookAttackDown);
			break;
		case Protocol::MOVE_DIR_LEFT:
		case Protocol::MOVE_DIR_RIGHT:
			GetSprite()->SetFlipbook(FlipbookAttackSide);
			break;
		}
		break;
	case Protocol::CREATURE_STATE_DEAD:
		break;
	default:
		break;
	}
}

bool AMiniDSPlayer::IsMyPlayer()
{
	return Cast<AMyMiniDSPlayer>(this) != nullptr;
}

void AMiniDSPlayer::SetMoveDir(Protocol::MoveDir Dir)
{
	if (PlayerInfo->dir() == Dir)
		return;

	PlayerInfo->set_dir(Dir);
}

void AMiniDSPlayer::SetState(Protocol::CreatureState State)
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

	SetState(Info.state());
	SetMoveDir(Info.dir());
}
