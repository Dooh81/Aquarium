// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WebSocketGameInstance.h"
#include "BrokenGlass.generated.h"

UCLASS()
class AQUARIUM_API ABrokenGlass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrokenGlass();

protected:
	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;

public:	
	// Called every frame
// 	virtual void Tick(float DeltaTime) override;





public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Direction")
		EInferredDir Direction = EInferredDir::None;

};
