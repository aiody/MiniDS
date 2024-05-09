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
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"

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
		SetMoveDir(Protocol::MOVE_DIR_DOWN);
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
				AddMovementInput(MoveDir);
		}
		else
		{
			AddMovementInput(MoveDir);
		}
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

	if (State == Protocol::CREATURE_STATE_ATTACK)
	{
		GetAnimationComponent()->GetAnimInstance()->JumpToNode(TEXT("Attacked"));
	}
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

FVector2D AMiniDSPlayer::BP_GetMoveDir()
{
	Protocol::MoveDir MoveDir = PlayerInfo->dir();
	FVector2D Vector2D;
	switch (MoveDir)
	{
	case Protocol::MOVE_DIR_NONE:
		Vector2D = FVector2D(0.f, -1.f);
		break;
	case Protocol::MOVE_DIR_UP:
		Vector2D = FVector2D(0.f, 1.f);
		break;
	case Protocol::MOVE_DIR_DOWN:
		Vector2D = FVector2D(0.f, -1.f);
		break;
	case Protocol::MOVE_DIR_LEFT:
		Vector2D = FVector2D(-1.f, 0.f);
		break;
	case Protocol::MOVE_DIR_RIGHT:
		Vector2D = FVector2D(1.f, 0.f);
		break;
	default:
		Vector2D = FVector2D(0.f, 0.f);
	}
	return Vector2D;
}

ECreatureState AMiniDSPlayer::BP_GetState()
{
	Protocol::CreatureState State = PlayerInfo->state();
	return static_cast<ECreatureState>(static_cast<int>(State));
}

void AMiniDSPlayer::BP_ReleaseAttack()
{
	SetState(Protocol::CREATURE_STATE_IDLE);
}
