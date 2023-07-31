// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OceanFish.h"
#include "OF_Kids.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUM_API AOF_Kids : public AOceanFish
{
	GENERATED_BODY()
	

public:
	AOF_Kids();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USplineComponent* LeftSplineComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USplineComponent* RightSplineComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USplineComponent* CenterSplineComp;


// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
// 		class USplineComponent* LeftAwaySplineComp;
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
// 		class USplineComponent* RightAwaySplineComp;
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
// 		class USplineComponent* CenterAwaySplineComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

protected:
	virtual void PatrolProcess() override;

	virtual void AttackProcess() override;

	virtual void StunnedProcess() override;

	virtual void ReturnProcess() override;


	// timer
	void SetReturnState();

public:
	virtual void SetAttackTargetLocation (const FVector location) override;



public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float Speed = 100;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		uint8 LeftStunnedIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		uint8 RightStunnedIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		uint8 CenterStunnedIndex = 0;



protected:
	virtual void SetWebSocketArray();


	virtual void SetWebSocketQueueBool();

public:
	UPROPERTY(BlueprintReadWrite);
		float SplineMovementDistance = 0;

	UPROPERTY(BlueprintReadWrite);
		USplineComponent* currentSpline;

	UPROPERTY(BlueprintReadWrite);
		uint8 CurrentStunnedIndex = 0;

protected:

		FVector DefaultPosition;

		float FloatingTime = 0;


};