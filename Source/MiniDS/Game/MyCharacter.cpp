// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "PaperFlipbookComponent.h"

AMyCharacter::AMyCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->AddLocalRotation(FRotator(-35.f, 0, 0));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMyCharacter::BeginPlay()
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

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyCharacter::ReleaseMove);
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
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

		UpdateAnimation(MovementVector);
	}
}

void AMyCharacter::ReleaseMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		UpdateAnimation(MovementVector);
	}
}

void AMyCharacter::UpdateAnimation(const FVector2D MovementVector)
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
