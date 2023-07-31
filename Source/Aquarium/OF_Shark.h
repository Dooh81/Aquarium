// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OceanFish.h"
#include <Components/SplineComponent.h>

#include "OF_Shark.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUM_API AOF_Shark : public AOceanFish
{
	GENERATED_BODY()
	

public:


	AOF_Shark();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USplineComponent* SplineComp;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


private:
	virtual void PatrolProcess() override;

	virtual void AttackProcess() override;

	virtual void StunnedProcess() override;

	virtual void ReturnProcess() override;



public:
	virtual void SetAttackTargetLocation(const FVector location) override;


	UFUNCTION(BlueprintCallable)
	void StopFish();

	UFUNCTION(BlueprintCallable)
	void ToStun();


	UFUNCTION(BlueprintCallable)
	void FeedProcess();

protected:
	UFUNCTION()
		void ResetWebSocketBool();

private:
	float CurrentStunnedTime = 0;


	bool ReturnProcessTargetOnceBool = false;

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float AttackTurnLerpAlphaWeight = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float PatrolMoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float AttackReadySpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float AttackMoveSpeed = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float StartStunDistance = 150.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float AttackLerpDistance = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float StunnedMoveSpeed = 20.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ReturnTurnLerpAlphaWeight = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ReturnPatrolDistance = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ReturnPatrolBoostDistance = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ReturnPatrolReadySpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ReturnPatrolSpeed = 1000;





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		bool AnimAttackBool = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		bool AnimReattackBool = false;






public:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		TSubclassOf<class AActor> BrokenGlass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		TSubclassOf<UCameraShakeBase> CameraShake;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		TSubclassOf<UCameraShakeBase> CameraShake_Prrr;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		bool bGlassRotateRandomly = false;
};
