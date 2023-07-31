// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OF_Kids.h"
#include "OFK_ClownFish.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUM_API AOFK_ClownFish : public AOF_Kids
{
	GENERATED_BODY()
	

public:
	AOFK_ClownFish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




protected:
	virtual void PatrolProcess() override;
	virtual void AttackProcess() override;
	virtual void StunnedProcess() override;
	virtual void ReturnProcess() override;



protected:
	virtual void SetWebSocketArray() override;



public: 
	UPROPERTY(BlueprintReadWrite)
		bool OnMoving = false;
};
