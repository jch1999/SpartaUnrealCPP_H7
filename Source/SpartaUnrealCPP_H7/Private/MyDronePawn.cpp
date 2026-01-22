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
	// X,Y 속도 처리
	Super::UpdateVelocity(DeltaTime);

	// Z 속도 처리
	if (FMath::IsNearlyEqual(ZInput, 1.0f))
	{
		const FVector ZAccel = FVector(0.f, 0.f, ZAcceleration);
		Velocity += ZAccel*DeltaTime;
	}
	else if (FMath::IsNearlyEqual(ZInput, -1.0f))
	{
		const FVector ZAccel = FVector(0.f, 0.f, -ZAcceleration);
		Velocity += ZAccel * DeltaTime;
	}
	else
	{
		float NewZ= FMath::Max(Velocity.Z - Deceleration * DeltaTime, 0.f);
		Velocity.Z = NewZ;
	}

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

