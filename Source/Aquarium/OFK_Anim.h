// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OF_Kids.h"
#include "OFK_Anim.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUM_API AOFK_Anim : public AOF_Kids
{
	GENERATED_BODY()
	


protected:
	virtual void BeginPlay() override;

protected:
	virtual void PatrolProcess()	override;
	virtual void AttackProcess()	override;
	virtual void StunnedProcess()	override;
	virtual void ReturnProcess()	override;


public:
	virtual void SetAttackTargetLocation(const FVector location) override;


//private:
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimMontage* AnimMontage1;	// left
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimMontage* AnimMontage2;	// center
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimMontage* AnimMontage3;	// right



public:
	UPROPERTY(EditAnywhere, Category = "Anim")
	float LeftSpeed = 1;
	UPROPERTY(EditAnywhere, Category = "Anim")
	float CenterSpeed = 1;
	UPROPERTY(EditAnywhere, Category = "Anim")
	float RightSpeed = 1;


public:
	UFUNCTION(BlueprintCallable)
	void SetStateReturn(const EOceanFishBehaviorState value);


};