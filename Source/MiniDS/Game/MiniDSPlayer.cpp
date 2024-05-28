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
#include "AnimSequences/Players/PaperZDAnimPlayer.h"
#include "DrawDebugHelpers.h"
#include "MiniDS.h"
#include "FloatingDamage.h"

AMiniDSPlayer::AMiniDSPlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(75.f, 155.f);

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

	ObjectInfo = new Protocol::ObjectInfo();
	CreatureInfo = new Protocol::CreatureInfo();
	ObjectInfo->set_allocated_creature_info(CreatureInfo);
	StatInfo = new Protocol::StatInfo();
	CreatureInfo->set_allocated_stat_info(StatInfo);
	PosInfo = new Protocol::PosInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);
	DestInfo = new Protocol::PosInfo();
}

AMiniDSPlayer::~AMiniDSPlayer()
{
	delete ObjectInfo;
	ObjectInfo = nullptr;
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
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
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
	if (PosInfo->dir() == Dir)
		return;

	PosInfo->set_dir(Dir);
}

void AMiniDSPlayer::SetState(Protocol::CreatureState State)
{
	if (GetState() == State)
		return;

	CreatureInfo->set_state(State);

	if (State == Protocol::CREATURE_STATE_ATTACK)
	{
		GetAnimationComponent()->GetAnimInstance()->JumpToNode(TEXT("AttackSignal"));
	}

	if (State == Protocol::CREATURE_STATE_HIT)
	{
		GetAnimationComponent()->GetAnimInstance()->JumpToNode(TEXT("HitSignal"));
	}

	if (State == Protocol::CREATURE_STATE_DEATH)
	{
		GetAnimationComponent()->GetAnimInstance()->JumpToNode(TEXT("DeathSignal"));
	}
}

void AMiniDSPlayer::SetObjectInfo(const Protocol::ObjectInfo& Info)
{
	ObjectInfo->MergeFrom(Info);
}

void AMiniDSPlayer::SetPosInfo(const Protocol::PosInfo& Info)
{
	PosInfo->MergeFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void AMiniDSPlayer::SetDestInfo(const Protocol::PosInfo& Info)
{
	DestInfo->MergeFrom(Info);

	SetMoveDir(Info.dir());
}

void AMiniDSPlayer::Hit(float Damage)
{
	float CurHp = StatInfo->hp();
	StatInfo->set_hp(CurHp - Damage);
	UE_LOG(LogTemp, Log, TEXT("Remain Hp : %f"), StatInfo->hp());

	FVector SpawnLocation(PosInfo->x(), PosInfo->y(), PosInfo->z() + 100.f);
	FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
	AFloatingDamage* FloatingDamage = GetWorld()->SpawnActorDeferred<AFloatingDamage>(FloatingDamageClass, SpawnTransform);
	if (FloatingDamage != nullptr)
	{
		FloatingDamage->Damage = Damage;
		FloatingDamage->FinishSpawning(SpawnTransform);
		UE_LOG(LogTemp, Log, TEXT("Spawn Floating Text!"));
	}
}

FVector2D AMiniDSPlayer::GetMoveDirVec2D()
{
	Protocol::MoveDir MoveDir = PosInfo->dir();
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

ECreatureState AMiniDSPlayer::GetStateWrapped()
{
	Protocol::CreatureState State = GetState();
	return static_cast<ECreatureState>(static_cast<int>(State));
}

void AMiniDSPlayer::BackToIdle()
{
	SetState(Protocol::CREATURE_STATE_IDLE);
}

void AMiniDSPlayer::AnimNotify_Attack()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(EName::None, false, this);

	float AttackRange = 200.f;
	float AttackRadius = 10.f;
	FVector MoveDir = FVector(GetMoveDirVec2D().Y, GetMoveDirVec2D().X, 0);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + MoveDir * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	{
		// For Debug
		FVector Vec = MoveDir * AttackRange;
		FVector Center = GetActorLocation() + Vec * 0.5f;
		float HalfHeight = AttackRange * 0.5f + AttackRadius;
		FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 2.f);
	}

	if (IsMyPlayer() && bResult && HitResult.GetActor()->IsValidLowLevel())
	{
		AActor* Actor = HitResult.GetActor();
		AMiniDSPlayer* OtherPlayer = Cast<AMiniDSPlayer>(Actor);

		if (OtherPlayer->GetState() == Protocol::CREATURE_STATE_DEATH)
			return;
		
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s(%d)"), *GetName(), ObjectInfo->object_id());
		UE_LOG(LogTemp, Log, TEXT("Attacked Actor : %s(%d)"), *OtherPlayer->GetName(), OtherPlayer->GetObjectInfo()->object_id());

		{
			Protocol::C_ATTACK AttackPkt;
			{
				AttackPkt.set_object_id(OtherPlayer->GetObjectInfo()->object_id());
			}
			SEND_PACKET(AttackPkt);
		}
	}
}
