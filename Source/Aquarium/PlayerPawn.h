// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
//#include "WebSocketGameInstance.h"

#include "PlayerPawn.generated.h"

UCLASS()
class AQUARIUM_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		class USceneComponent* ScComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		class UCameraComponent* CameraComp;
// 		class UCineCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		class UStaticMeshComponent* Screen;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		class UStaticMeshComponent* LeftPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		class UStaticMeshComponent* RightPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		class UStaticMeshComponent* CenterPosition;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// protected:
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
// 	class UInputAction* MouseClickAction;


protected:
	void GetScreenClickPosition();



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Linetrace")
		float LineDistance = 1000;


	


	void LocalScreenToWorld(FVector2D ScreenPosition, FVector& WorldLocation, FVector& WorldDirection);


	UFUNCTION(BlueprintCallable)
		void GetScreenClickPositionSocket(FVector2D value);




	// WebSocket delegate
	UFUNCTION(BlueprintCallable)
		void CallWebSocketDelegate(EInferredDir dirValue, int ArrayIndex);



public:
// 	UPROPERTY()
// 	class AActor* OceanFish;

	UPROPERTY()
	class UWebSocketGameInstance* webSocketInstance;






public:
	UPROPERTY(EditAnywhere, Category = "Category")
	int OcenFishArrayIndex = 2;

};
