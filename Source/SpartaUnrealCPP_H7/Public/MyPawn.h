#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTAUNREALCPP_H7_API AMyPawn : public APawn
{
	GENERATED_BODY()
public:
	AMyPawn();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	virtual void Turn(const FInputActionValue& Value);

	virtual void UpdateVelocity(float DeltaTime);
	virtual void AddGravity(float DeltaTime);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Acceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Deceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float StopThreshold; // 무시 가능 범위

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Look")
	float LookSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float GravityScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	bool bUseGravity;

	FVector2D MoveInput;
};
