// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OF_Kids.h"
#include "OFK_ClownFishFactory.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUM_API AOFK_ClownFishFactory : public AOF_Kids
{
	GENERATED_BODY()

public:
	AOFK_ClownFishFactory();
	

	UPROPERTY(EditAnywhere, Category = "ChildActors")
		class USceneComponent* FishSceneComp;

	UPROPERTY(EditAnywhere, Category = "ChildActors")
	TArray<class UChildActorComponent*> ClownFishArray;

	UPROPERTY(EditAnywhere, Category = "ChildActors")
		int FishCount = 50;

	 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetWebSocketArray();



protected:
	virtual void PatrolProcess() override;
	virtual void AttackProcess() override;
	virtual void StunnedProcess() override;
	virtual void ReturnProcess() override;



	void CheckChildMovingBool();




protected:
		virtual void SetAttackTargetLocation(const FVector location) override;


		virtual void SetWebSocketQueueBool() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior State")
		EInferredDir FactoryInferredDirection = EInferredDir::Left;




public:
	FTimerHandle ReInferTimer;

};
