// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Pig.h"
#include "Components/CapsuleComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MiniDS.h"

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

	if (bResult && HitResult.GetActor()->IsValidLowLevel())
	{
		AActor* Actor = HitResult.GetActor();
		ACreature* OtherPlayer = Cast<ACreature>(Actor);

		if (OtherPlayer->GetState() == Protocol::CREATURE_STATE_DEATH)
			return;

		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s(%d)"), *GetName(), ObjectInfo->object_id());
		UE_LOG(LogTemp, Log, TEXT("Attacked Actor : %s(%d)"), *OtherPlayer->GetName(), OtherPlayer->GetObjectInfo()->object_id());

		{
			Protocol::C_ATTACK AttackPkt;
			{
				AttackPkt.set_from(ObjectInfo->object_id());
				AttackPkt.set_to(OtherPlayer->GetObjectInfo()->object_id());
			}
			SEND_PACKET(AttackPkt);
		}
	}
}
