#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.h"
#include "MyDronePawn.generated.h"

struct FInputActionValue;

UCLASS()
class SPARTAUNREALCPP_H7_API AMyDronePawn : public AMyPawn
{
	GENERATED_BODY()

public:
	AMyDronePawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void OnUp(const FInputActionValue& Value);
	UFUNCTION()
	void OnDown(const FInputActionValue& Value);
	UFUNCTION()
	void OnRoll(const FInputActionValue& Value);

	virtual void UpdateVelocity(float DeltaTime) override;
	virtual void UpdateRoll(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Move")
	float ZAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float ZDeceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float RollSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	float ZInput;

	FRotator RollInput;
};
