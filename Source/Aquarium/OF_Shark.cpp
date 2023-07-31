// Fill out your copyright notice in the Description page of Project Settings.


#include "OF_Shark.h"
#include "BrokenGlass.h"

#include "WebSocketGameInstance.h"
#include <Components/SceneComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/SplineComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerPawn.h"
#include "TimerManager.h"

#include <random>
#include <iostream>


AOF_Shark::AOF_Shark()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
}


void AOF_Shark::BeginPlay()
{
	Super::BeginPlay();

	//	loc
	SplineComp->SetWorldLocation(GetActorLocation());
	//	rot
	SplineComp->SetWorldRotation(GetActorRotation());



	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
		webSocketInstance->OceanFishes[0] = this;
}



void AOF_Shark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOF_Shark::PatrolProcess()
{
	FVector moveDirection;
	float closestKey;
	FVector targetLocation;

	if (SplineComp->IsClosedLoop())
	{
		closestKey = SplineComp->FindInputKeyClosestToWorldLocation(GetActorLocation());
		auto targetexpectdistance = SplineComp->GetDistanceAlongSplineAtSplineInputKey(closestKey) + 100;
		targetLocation = SplineComp->GetLocationAtDistanceAlongSpline(targetexpectdistance, ESplineCoordinateSpace::World);
		//targetLocation = targetLocation + FVector(0, 0, 0);
		moveDirection = targetLocation - GetActorLocation();


		// add movement input
		moveDirection.Normalize();
		AddMovementInput(moveDirection, 1);
	}
}

void AOF_Shark::AttackProcess()
{
	// for DeepLearning L/R/C Test
	switch (InferredDirection)
	{
		case EInferredDir::Left:
			AttackTargetLocation = LeftCoordinate;
		break;
		case EInferredDir::Right:
			AttackTargetLocation = RightCoordinate;
		break;
		case EInferredDir::Center:
			AttackTargetLocation = CenterCoordinate;
		break;
		default:
		break;
	}




	FVector moveDirection;

	moveDirection = AttackTargetLocation - GetActorLocation();
	moveDirection.Normalize();


	// turn to target -> lerp
	float dot = FVector::DotProduct(GetActorForwardVector(), moveDirection);
	float AcosAngle = FMath::Acos(dot);
	float angle = FMath::RadiansToDegrees(AcosAngle);

	if (angle > 5)
	{
		moveDirection = FMath::Lerp(GetActorForwardVector(), moveDirection, GetWorld()->DeltaTimeSeconds * AttackTurnLerpAlphaWeight);
		GetCharacterMovement()->MaxFlySpeed = PatrolMoveSpeed;
	}
	else
	{
		if (FVector::Dist2D(GetActorLocation(), AttackTargetLocation) > AttackLerpDistance)
			GetCharacterMovement()->MaxFlySpeed = AttackReadySpeed;
		else
			GetCharacterMovement()->MaxFlySpeed = AttackMoveSpeed;
	}

	AddMovementInput(moveDirection, 1);





	auto TargetDistance = FVector::Dist(AttackTargetLocation, GetActorLocation());

	if (TargetDistance < StartStunDistance + 400)
		AnimAttackBool = true;


	if (TargetDistance < StartStunDistance)
	{
		StopFish();

		ToStun();
	}



}

void AOF_Shark::StunnedProcess()
{
	CurrentStunnedTime += GetWorld()->DeltaTimeSeconds;


	if (CurrentStunnedTime > 8)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxFlySpeed = PatrolMoveSpeed;
		CurrentStunnedTime = 0;

		BehaviorState = EOceanFishBehaviorState::Return;
		AnimReattackBool = false;
	}
	else if (CurrentStunnedTime < 5)
	{

		// anim reattack
		if (CurrentStunnedTime > 1 && CurrentStunnedTime < 2)
			AnimReattackBool = true;
		else
		{
			FVector moveDirection;
			moveDirection = -GetActorForwardVector();		// move backward
			GetCharacterMovement()->MaxFlySpeed = StunnedMoveSpeed;

			AddMovementInput(moveDirection, 1);
		}
	}
}

void AOF_Shark::ReturnProcess()
{
	FVector moveDirection;
	//float closestKey;
	FVector targetLocation;

	targetLocation = SplineComp->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);

	DrawDebugSphere(GetWorld(), targetLocation, 10, 10, FColor::Red, true, -1, 1, 1);

	moveDirection = targetLocation - GetActorLocation();
	moveDirection.Normalize();



	float dot = FVector::DotProduct(GetActorForwardVector(), moveDirection);
	float AcosAngle = FMath::Acos(dot);
	float angle = FMath::RadiansToDegrees(AcosAngle);

	if ((angle >= 5))
	{
		moveDirection = FMath::Lerp(GetActorForwardVector(), moveDirection, GetWorld()->DeltaTimeSeconds * ReturnTurnLerpAlphaWeight);
		GetCharacterMovement()->MaxFlySpeed = ReturnPatrolReadySpeed;
	}
	else
	{
		auto DistFromSpline = FVector::Dist(GetActorLocation(), AttackTargetLocation);
		if (DistFromSpline < ReturnPatrolBoostDistance)
			GetCharacterMovement()->MaxFlySpeed = ReturnPatrolReadySpeed;
		else
			GetCharacterMovement()->MaxFlySpeed = ReturnPatrolSpeed;
	}




	// add movement input
	AddMovementInput(moveDirection, 1);



	// on destination -> Set Patrol Mode
	if (FVector::Dist(GetActorLocation(), targetLocation) < ReturnPatrolDistance)
	{
		//ReturnProcessTargetOnceBool = false;

		auto TargetSpeed = FMath::Lerp(GetCharacterMovement()->Velocity, GetActorForwardVector() * PatrolMoveSpeed, GetWorld()->DeltaTimeSeconds);
		GetCharacterMovement()->Velocity = TargetSpeed;
		GetCharacterMovement()->MaxFlySpeed = PatrolMoveSpeed;

		// set patrol mode
		if (FVector::Dist(GetActorLocation(), targetLocation) < (ReturnPatrolDistance / 2))
		{

			// Timer
			FTimerHandle ResetQueueHandle;
			GetWorld()->GetTimerManager().SetTimer(ResetQueueHandle, this, &AOF_Shark::ResetWebSocketBool, ResetDelayTime, false, ResetDelayTime);
			
			BehaviorState = EOceanFishBehaviorState::Patrol;
		}
	}
}



void AOF_Shark::SetAttackTargetLocation(const FVector location)
{
	Super::SetAttackTargetLocation(location);



	GetCharacterMovement()->MaxFlySpeed = AttackMoveSpeed;


	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
	{
		if(webSocketInstance->bSharkFeed)
		{
			auto Direcrtion = webSocketInstance->GetEnumerationToString(InferredDirection);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Direcrtion + "Feed Shark");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Pronun");
		}
	}

}


void AOF_Shark::StopFish()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->Velocity = FVector(0, 0, 0);
}

void AOF_Shark::ToStun()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// set location(knock back)               
	SetActorLocation(GetActorLocation() - (GetActorForwardVector() * 10));



	// spawn broken glass effect
	if (BrokenGlass)
	{
		auto Player = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass()));
		//FVector SpawnLocation = AttackTargetLocation - (-GetActorForwardVector() * 70);
		FVector SpawnLocation = FVector(-645, -32, 224) - Player->GetActorForwardVector() * 40;
		switch (InferredDirection)
		{
		case EInferredDir::Left:
			SpawnLocation = LeftCoordinate - Player->GetActorForwardVector() * 40;
			break;
		case EInferredDir::Right:
			SpawnLocation = RightCoordinate - Player->GetActorForwardVector() * 40;
			break;
		case EInferredDir::Center:
			SpawnLocation = CenterCoordinate - Player->GetActorForwardVector() * 40;
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("%f   %f   %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);


		// 난수 생성
		int RandomInt = 180;
		if (bGlassRotateRandomly)
		{
			std::random_device rd;
			std::mt19937 MyRandomEngine(rd());
			std::uniform_int_distribution<int> distribution(0, 360);
			RandomInt = distribution(MyRandomEngine);

			UE_LOG(LogTemp, Warning, TEXT("%d"), RandomInt);
		}

		// set rotation
		FRotator SpawnRotation = Player->GetActorRotation() + FRotator(180, 0, RandomInt);
		auto SpawnedActor = GetWorld()->SpawnActor<AActor>(BrokenGlass, SpawnLocation, SpawnRotation);
		auto BrokenGlassActor = Cast<ABrokenGlass>(SpawnedActor);
		BrokenGlassActor->Direction = InferredDirection;

	}



	// camera shake
	if(CameraShake != nullptr)
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShake, 1.f);
	if(CameraShake_Prrr != nullptr)
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShake_Prrr, 1.f);



	BehaviorState = EOceanFishBehaviorState::Stunned;
	AnimAttackBool = false;
}

void AOF_Shark::FeedProcess()
{

}

void AOF_Shark::ResetWebSocketBool()
{
	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
	{
		webSocketInstance->OnInferDirectionShark = true;
		webSocketInstance->bSharkFeed = false;
		webSocketInstance->ServerSendEndMessage("End", "Shark");
		//webSocketInstance->ServerSendMessage2("ID", webSocketInstance->WebSocketID);

	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Shark Timer Called(ResetWebSocketBool)");
	UE_LOG(LogTemp, Warning, TEXT("Shark Timer Called(ResetWebSocketBool)"));

}









