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

	MoveInput = FVector2D::ZeroVector;
	MaxSpeed = 600.0f;
	Acceleration = 2000.0f;
	Deceleration = 2500.0f;
	StopThreshold = 5.f;
	LookSpeed = 1.0f;
	GravityScale = -9.8f;
	bUseGravity = true;
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateVelocity(DeltaTime);
	AddGravity(DeltaTime);

	if (!Velocity.IsNearlyZero())
	{
		AddActorLocalOffset(Velocity * DeltaTime, true);
	}
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
	MoveInput = Value.Get<FVector2D>();
}

void AMyPawn::Turn(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.IsNearlyZero()) return;

	AddActorLocalRotation(FRotator(0, LookValue.X, 0.0f) * LookSpeed);
	
	SpringArmComp->AddLocalRotation(FRotator(LookValue.Y * LookSpeed, 0, 0));
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
	SpringArmRotation.Pitch = FMath::ClampAngle(SpringArmRotation.Pitch, -60.f, 60.f);
	SpringArmComp->SetRelativeRotation(SpringArmRotation);
}

void AMyPawn::UpdateVelocity(float DeltaTime)
{
	if (!MoveInput.IsNearlyZero())
	{
		// GetClampedToMaxSize - 백터의 크기가 일정 값을 넘지못하게 제한하는 함수
		const FVector LocalAccel = FVector(MoveInput.X, MoveInput.Y, 0.f).GetClampedToMaxSize(1.f) * Acceleration;
		Velocity += LocalAccel * DeltaTime;
	}
	else
	{
		const float Speed = Velocity.Size();
		const float NewSpeed = FMath::Max(Speed - Deceleration * DeltaTime, 0.f);

		// KINDA_SMALL_NUMBER : 0으로 정규화하다가 이상해지는 걸 피하기 위한 안전장치
		Velocity = (Speed > KINDA_SMALL_NUMBER)
			? Velocity.GetSafeNormal() * NewSpeed
			: FVector::ZeroVector;
	}

	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
	if (Velocity.Size() < StopThreshold)
	{
		Velocity = FVector::ZeroVector;
	}
	MoveInput = FVector2D::ZeroVector;
}

void AMyPawn::AddGravity(float DeltaTime)
{
	if (!bUseGravity) return;

	Velocity.Z += GravityScale * DeltaTime;
}

