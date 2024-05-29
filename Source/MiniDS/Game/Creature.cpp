// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Creature.h"
#include "MyMiniDSPlayer.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"
#include "FloatingDamage.h"

ACreature::ACreature()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	ObjectInfo = new Protocol::ObjectInfo();
	CreatureInfo = new Protocol::CreatureInfo();
	ObjectInfo->set_allocated_creature_info(CreatureInfo);
	StatInfo = new Protocol::StatInfo();
	CreatureInfo->set_allocated_stat_info(StatInfo);
	PosInfo = new Protocol::PosInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);
	DestInfo = new Protocol::PosInfo();
}

ACreature::~ACreature()
{
	delete ObjectInfo;
	ObjectInfo = nullptr;
	delete DestInfo;
	DestInfo = nullptr;
}

void ACreature::BeginPlay()
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

void ACreature::Tick(float DeltaSeconds)
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

bool ACreature::IsMyPlayer()
{
	return Cast<AMyMiniDSPlayer>(this) != nullptr;
}

void ACreature::SetMoveDir(Protocol::MoveDir Dir)
{
	if (PosInfo->dir() == Dir)
		return;

	PosInfo->set_dir(Dir);
}

void ACreature::SetState(Protocol::CreatureState State)
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

void ACreature::SetObjectInfo(const Protocol::ObjectInfo& Info)
{
	ObjectInfo->MergeFrom(Info);
}

void ACreature::SetPosInfo(const Protocol::PosInfo& Info)
{
	PosInfo->MergeFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void ACreature::SetDestInfo(const Protocol::PosInfo& Info)
{
	DestInfo->MergeFrom(Info);

	SetMoveDir(Info.dir());
}

void ACreature::Hit(float Damage)
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

FVector2D ACreature::GetMoveDirVec2D()
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

ECreatureState ACreature::GetStateWrapped()
{
	Protocol::CreatureState State = GetState();
	return static_cast<ECreatureState>(static_cast<int>(State));
}

void ACreature::BackToIdle()
{
	SetState(Protocol::CREATURE_STATE_IDLE);
}
