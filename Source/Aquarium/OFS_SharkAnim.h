// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OF_Shark.h"
#include "OFS_SharkAnim.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUM_API AOFS_SharkAnim : public AOF_Shark
{
	GENERATED_BODY()
	
public:
	AOFS_SharkAnim();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
		class USkeletalMeshComponent* FeedCenter1;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
// 		class USkeletalMeshComponent* Feed2;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaTime) override;


private:
	virtual void PatrolProcess() override;

	virtual void AttackProcess() override;

	virtual void StunnedProcess() override;

	virtual void ReturnProcess() override;



	//

public:
	virtual void SetAttackTargetLocation(const FVector location) override;

	UFUNCTION(BlueprintCallable)
	void SetStateReturn(const EOceanFishBehaviorState value);
	UFUNCTION()
	void InitInteractionBool();

	UFUNCTION(BlueprintCallable)
	void PlayFeedAnim();

public:
	UPROPERTY(BlueprintReadWrite)
		bool bFeedLeft = false;
	UPROPERTY(BlueprintReadWrite)
		bool bFeedCenter = false;
	UPROPERTY(BlueprintReadWrite)
		bool bFeedRight = false;

	UPROPERTY(BlueprintReadWrite)
		bool bPronunLeft = false;
	UPROPERTY(BlueprintReadWrite)
		bool bPronunCenter = false;
	UPROPERTY(BlueprintReadWrite)
		bool bPronunRight = false;


public:
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimMontage* FeedCenterAnimMontage1;	// left


private:
	class UAnimMontage* CurrentAnim;

};
