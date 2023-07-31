// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"

#include "Camera/CameraComponent.h"
#include "OceanFish.h"
#include "OF_Shark.h"
#include "WebSocketGameInstance.h"
//#include "Camera/CineCameraComponent.h"
//#include "CineCameraComponent.h"

#include <GameFramework/CharacterMovementComponent.h>
#include "OFK_ClownFishFactory.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ScComp = CreateDefaultSubobject<USceneComponent>(TEXT("ScComp"));
	SetRootComponent(ScComp);

// 	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp-> FieldOfView = 45;

	Screen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	Screen->SetupAttachment(RootComponent);

	LeftPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPosition"));
	LeftPosition->SetupAttachment(Screen);
	LeftPosition->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftPosition->bHiddenInGame = true;

	RightPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPosition"));
	RightPosition->SetupAttachment(Screen);
	RightPosition->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightPosition->bHiddenInGame = true;

	CenterPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterPosition"));
	CenterPosition->SetupAttachment(Screen);
	CenterPosition->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CenterPosition->bHiddenInGame = true;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	

	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(true);


	// bind delegate
	webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
		webSocketInstance->EventDelegateOneParam.BindUFunction(this, FName("CallWebSocketDelegate"));



	// check websocket index is valid
	if(! webSocketInstance->OceanFishes.IsValidIndex(OcenFishArrayIndex))
		return;

	auto AttackOceanFish = Cast<AOceanFish>(webSocketInstance->OceanFishes[OcenFishArrayIndex]);
	if (AttackOceanFish)
	{
		if (auto ClownFishFactory = Cast<AOFK_ClownFishFactory>(AttackOceanFish))
			ClownFishFactory->FactoryInferredDirection = EInferredDir::Left;
		else
			AttackOceanFish->InferredDirection = EInferredDir::Left;;

	}
}

// Called every frame
// void APlayerPawn::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 
// 
// }

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &APlayerPawn::GetScreenClickPosition);

}

void APlayerPawn::GetScreenClickPosition()
{
	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldLocation;
	FVector WorldDirection;

// 	FVector2D dummy(100,100);
// 	LocalScreenToWorld(dummy, WorldLocation, WorldDirection);
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%.f, %.f, %.f"), WorldLocation.X, WorldLocation.Y, WorldLocation.Z));


	FHitResult HitInfo;
	FVector LineEndPosition = WorldLocation + WorldDirection * LineDistance;

	GetWorld()->LineTraceSingleByChannel(HitInfo, WorldLocation, LineEndPosition, ECollisionChannel::ECC_Visibility);
	if (HitInfo.GetActor())
	{
		FVector HitImpactPoint = HitInfo.ImpactPoint;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%.f, %.f, %.f"), HitImpactPoint.X, HitImpactPoint.Y, HitImpactPoint.Z));
		
		DrawDebugLine(GetWorld(), WorldLocation, HitImpactPoint, FColor::Red, false, 5, 10, 0.1);
		DrawDebugSphere(GetWorld(), HitImpactPoint, 0.1, 10,  FColor::Green, false, 5, 0, 0.1);

		if (webSocketInstance != nullptr)
		{
			// check websocket index is valid
			if (!webSocketInstance->OceanFishes.IsValidIndex(OcenFishArrayIndex))
				return;
									
								
			if (webSocketInstance->OceanFishes[OcenFishArrayIndex] != nullptr)
			{
				auto AttackOceanFish = Cast<AOceanFish>(webSocketInstance->OceanFishes[OcenFishArrayIndex]);
				if (AttackOceanFish->BehaviorState == EOceanFishBehaviorState::Patrol && AttackOceanFish)
					AttackOceanFish->SetAttackTargetLocation(HitImpactPoint);
			}
		}
	}
}

void APlayerPawn::LocalScreenToWorld(FVector2D ScreenPosition, FVector& WorldLocation, FVector& WorldDirection)
{
	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenPosition, WorldLocation, WorldDirection);



}

void APlayerPawn::GetScreenClickPositionSocket(FVector2D value)
{
	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldLocation;
	FVector WorldDirection;

	LocalScreenToWorld(value, WorldLocation, WorldDirection);
	//PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);


	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%.f, %.f, %.f"), WorldLocation.X, WorldLocation.Y, WorldLocation.Z));


	FHitResult HitInfo;
	FVector LineEndPosition = WorldLocation + WorldDirection * LineDistance;

	GetWorld()->LineTraceSingleByChannel(HitInfo, WorldLocation, LineEndPosition, ECollisionChannel::ECC_Visibility);
	if (HitInfo.GetActor())
	{
		FVector HitImpactPoint = HitInfo.ImpactPoint;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%.f, %.f, %.f"), HitImpactPoint.X, HitImpactPoint.Y, HitImpactPoint.Z));

		DrawDebugLine(GetWorld(), WorldLocation, HitImpactPoint, FColor::Red, false, 5, 10, 0.1);
		DrawDebugSphere(GetWorld(), HitImpactPoint, 0.1, 10, FColor::Green, false, 5, 0, 0.1);


		if (webSocketInstance->OceanFishes[OcenFishArrayIndex] != nullptr && webSocketInstance == nullptr)
		{
			auto AttackOceanFish = Cast<AOceanFish>(webSocketInstance->OceanFishes[OcenFishArrayIndex]);
			AttackOceanFish->SetAttackTargetLocation(HitImpactPoint);
		}
	}
}

void APlayerPawn::CallWebSocketDelegate(EInferredDir dirValue, int ArrayIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate On Player!"));

	OcenFishArrayIndex = ArrayIndex;

	if(webSocketInstance->OceanFishes[OcenFishArrayIndex] == nullptr || webSocketInstance == nullptr)
		return;





	auto AttackOceanFish = Cast<AOceanFish>(webSocketInstance->OceanFishes[OcenFishArrayIndex]);
	if(AttackOceanFish)
	{
		if (auto ClownFishFactory = Cast<AOFK_ClownFishFactory>(AttackOceanFish))
			ClownFishFactory->FactoryInferredDirection = dirValue;
		else 
			AttackOceanFish->InferredDirection = dirValue;

	}




	switch (dirValue)
	{	// 일단 좌표 이동 말고는 SetAttackTargetLocation 함수를 타기만 하면 됨 
		case EInferredDir::Left:
			AttackOceanFish->SetAttackTargetLocation(AttackOceanFish->LeftCoordinate);		
			break;
		case EInferredDir::Right:
			AttackOceanFish->SetAttackTargetLocation(AttackOceanFish->RightCoordinate);
			break;
		case EInferredDir::Center:
			AttackOceanFish->SetAttackTargetLocation(AttackOceanFish->CenterCoordinate);
			break;
	}
}

