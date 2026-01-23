#include "MyDronePawn.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFrameWork/SpringArmComponent.h"

AMyDronePawn::AMyDronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	if (SpringArmComp)
	{
		SpringArmComp->bInheritRoll = true;
	}

	RollSpeed = 10.0f;
	bUseGravity = false;
}

void AMyDronePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyDronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRoll(DeltaTime);
}

void AMyDronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->UpAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->UpAction, ETriggerEvent::Triggered, this, &AMyDronePawn::OnUp);
			}
			if (PlayerController->DownAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->DownAction, ETriggerEvent::Triggered, this, &AMyDronePawn::OnDown);
			}
			if (PlayerController->RollAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->RollAction, ETriggerEvent::Triggered, this, &AMyDronePawn::OnRoll);
			}
		}
	}
}

void AMyDronePawn::OnUp(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		ZInput = 1.0f;
	}
}

void AMyDronePawn::OnDown(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		ZInput = -1.0f;
	}
}

void AMyDronePawn::OnRoll(const FInputActionValue& Value)
{
	float RollValue = Value.Get<float>();
	RollInput = FRotator(0.0f, 0.0f, RollValue);
}

void AMyDronePawn::UpdateVelocity(float DeltaTime)
{
	FVector Accel = FVector::ZeroVector;
	if (!MoveInput.IsNearlyZero())
	{
		Accel = FVector(MoveInput.X, MoveInput.Y, 0.f).GetClampedToMaxSize(1.f) * Acceleration;
	}
	else
	{
		FVector2D XY(Velocity.X, Velocity.Y);
		const float XYSpeed = XY.Size();
		const float NewSpeed = FMath::Max(XYSpeed - Deceleration * DeltaTime, 0.f);

		// KINDA_SMALL_NUMBER : 0으로 정규화하다가 이상해지는 걸 피하기 위한 안전장치
		XY = (XYSpeed > KINDA_SMALL_NUMBER)
			? XY.GetSafeNormal() * NewSpeed
			: FVector2D::ZeroVector;
		Velocity.X = XY.X;
		Velocity.Y = XY.Y;
	}

	if (FMath::IsNearlyEqual(ZInput, 0.0f))
	{
		float NewZ = FMath::FInterpTo(Velocity.Z, 0.f, DeltaTime, Deceleration);
		Velocity.Z = NewZ;
	}
	else
	{
		Accel += FVector(0.f, 0.f, FMath::IsNearlyEqual(ZInput, 1.0f) ? ZAcceleration : -ZAcceleration);
	}
	Velocity += Accel * DeltaTime;

	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
	if (Velocity.Size() < StopThreshold)
	{
		Velocity = FVector::ZeroVector;
	}
	MoveInput = FVector2D::ZeroVector;
	ZInput = 0.0f;
}

void AMyDronePawn::UpdateRoll(float DeltaTime)
{
	if (!FMath::IsNearlyZero(RollInput.Vector().Size()))
	{
		AddActorLocalRotation(RollInput * RollSpeed * DeltaTime);
		RollInput = FRotator::ZeroRotator;
	}
}

