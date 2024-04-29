// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMiniDSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "MiniDS.h"

AMyMiniDSPlayer::AMyMiniDSPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->AddLocalRotation(FRotator(-35.f, 0, 0));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMyMiniDSPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyMiniDSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyMiniDSPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyMiniDSPlayer::Move);
	}
}

void AMyMiniDSPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Send 판정
	bool ForceSendPacket = false;

	if (LastDesiredInput != DesiredInput)
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}

	MovePacketSendTimer -= DeltaSeconds;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;
		{
			Protocol::PlayerInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
		}
		SEND_PACKET(MovePkt);
	}
}

void AMyMiniDSPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotaion = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotaion.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		SetMoveState(GetMovementState(MovementVector));
		UpdateAnimation(MovementVector);

		DesiredInput = MovementVector;
	}
}

Protocol::MoveState AMyMiniDSPlayer::GetMovementState(FVector2D MovementVector)
{
	Protocol::MoveState State = Protocol::MOVE_STATE_IDLE;

	if (MovementVector == FVector2D::Zero())
		State = Protocol::MOVE_STATE_IDLE;
	else if (MovementVector.X > 0)
		State = Protocol::MOVE_STATE_RUN_RIGHT;
	else if (MovementVector.X < 0)
		State = Protocol::MOVE_STATE_RUN_LEFT;
	else
	{
		if (MovementVector.Y > 0)
			State = Protocol::MOVE_STATE_RUN_UP;
		else
			State = Protocol::MOVE_STATE_RUN_DOWN;
	}

	return State;
}
