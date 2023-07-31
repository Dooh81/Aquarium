// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShakeBase.h"
#include "WebSocketGameInstance.h"

#include "OceanFish.generated.h"


UENUM(BlueprintType)
enum class EOceanFishBehaviorState : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Attack 	UMETA(DisplayName = "Attack"),
	Stunned UMETA(DisplayName = "Stunned"),
	Return UMETA(DisplayName = "Return"),
};

UENUM(BlueprintType)
enum class EInteractionMode : uint8
{
	Pronun		UMETA(DisplayName = "Pronun"),
	Feeding 	UMETA(DisplayName = "Feeding"),
};



UCLASS()
class AQUARIUM_API AOceanFish : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOceanFish();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	virtual void PatrolProcess();

	virtual void AttackProcess();

	virtual void StunnedProcess();

	virtual void ReturnProcess();

// public:
// 	UPROPERTY()
// 		bool bAttack = false;


public:
	UFUNCTION()
		virtual void SetAttackTargetLocation(const FVector location);

	UFUNCTION()
		FVector	GetAttackTargetLocation();


protected:
	FVector AttackTargetLocation;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior State")
		EInferredDir InferredDirection = EInferredDir::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior State")
		EOceanFishBehaviorState BehaviorState = EOceanFishBehaviorState::Patrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior State")
		EInteractionMode InteractionMode = EInteractionMode::Pronun;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		FVector LeftCoordinate = FVector(-649, -81, 224);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		FVector RightCoordinate = FVector(-641, 12, 225);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		FVector CenterCoordinate = FVector(-645, -32, 224);




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WebSocket")
		float ResetDelayTime = 5.f;

};
