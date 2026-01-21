#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyPlayerController.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	if (CapsuleComponent)
	{
		SetRootComponent(CapsuleComponent);
		CapsuleComponent->SetSimulatePhysics(false);
		CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	}

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	if (SkeletalMeshComp)
	{
		if (CapsuleComponent)
		{
			SkeletalMeshComp->SetupAttachment(CapsuleComponent);
		}
		SkeletalMeshComp->SetSimulatePhysics(false);
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (SpringArmComp)
	{
		if (SkeletalMeshComp)
		{
			SpringArmComp->SetupAttachment(CapsuleComponent);
		}
		SpringArmComp->TargetArmLength = 300.0f;
		SpringArmComp->TargetOffset = FVector::ZeroVector;
		SpringArmComp->bUsePawnControlRotation = false;
		// Roll 회전 막기
		SpringArmComp->bInheritRoll = false;
		SpringArmComp->bInheritYaw = true;
		SpringArmComp->bInheritPitch = true;
	}
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (CameraComp)
	{
		if (SpringArmComp)
		{
			CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
		}
		CameraComp->bUsePawnControlRotation = false;
	}

	LookSpeed = 1.0f;
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::Move);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AMyPawn::Turn);
			}
		}
	}
}

void AMyPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(MoveValue.X))
	{
		AddActorLocalOffset(GetActorForwardVector() * MoveValue.X);
	}
	if (!FMath::IsNearlyZero(MoveValue.Y))
	{
		AddActorLocalOffset(GetActorRightVector() * MoveValue.Y);
	}
}

void AMyPawn::Turn(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.IsNearlyZero()) return;

	AddActorLocalRotation(FRotator(0, LookValue.X, 0.0f) * LookSpeed);
	SpringArmComp->AddLocalRotation(FRotator(LookValue.Y * LookSpeed, 0, 0));
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
	SpringArmComp->SetRelativeRotation(FRotator(
		FMath::Clamp(SpringArmRotation.Pitch,0,80.0f),
		SpringArmRotation.Yaw,
		0.0f
	));
}

