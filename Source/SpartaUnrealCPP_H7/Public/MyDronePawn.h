#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.h"
#include "MyDronePawn.generated.h"

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

private:

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Move")
	float UpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float DownSpeed;
};
