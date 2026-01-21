#include "MyDronePawn.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

AMyDronePawn::AMyDronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMyDronePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyDronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

