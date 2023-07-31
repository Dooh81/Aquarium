// Fill out your copyright notice in the Description page of Project Settings.


#include "OceanFish.h"
#include "PlayerPawn.h"

#include "WebSocketGameInstance.h"
#include <Components/SceneComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/SplineComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>


// Sets default values
AOceanFish::AOceanFish()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxFlySpeed = 100.f;

}

// Called when the game starts or when spawned
void AOceanFish::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;


	auto PlayerController = GetWorld()->GetFirstPlayerController();
	auto Player = Cast<APlayerPawn>(PlayerController->GetPawn());
	if (Player)
	{
		LeftCoordinate = Player->LeftPosition->GetComponentLocation();
		RightCoordinate = Player->RightPosition->GetComponentLocation();
		CenterCoordinate = Player->CenterPosition->GetComponentLocation();
	}

// 	auto Player = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass()));
// 	if(Player != nullptr)
// 		Player->OceanFish = this;
}

// Called every frame
void AOceanFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	switch (BehaviorState)
	{
		case EOceanFishBehaviorState::Patrol:
		{
			PatrolProcess();

		}
		break;

		case EOceanFishBehaviorState::Attack:
		{
			AttackProcess();

		}
		break;

		case EOceanFishBehaviorState::Stunned:
		{
			StunnedProcess();

		}
		break;

		case EOceanFishBehaviorState::Return:
		{
			ReturnProcess();

		}
		break;

	}

}

// Called to bind functionality to input
void AOceanFish::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




void AOceanFish::PatrolProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("Patrol"));

}

void AOceanFish::AttackProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));

}

void AOceanFish::StunnedProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("Stunned"));

}

void AOceanFish::ReturnProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("Return"));

}





void AOceanFish::SetAttackTargetLocation(const FVector location)
{
	AttackTargetLocation = location;
	BehaviorState = EOceanFishBehaviorState::Attack;
}

FVector AOceanFish::GetAttackTargetLocation()
{
	return AttackTargetLocation;
}

