// Fill out your copyright notice in the Description page of Project Settings.


#include "OFK_ClownFish.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/SplineComponent.h"






AOFK_ClownFish::AOFK_ClownFish()
{

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxFlySpeed = 100.f;
}

void AOFK_ClownFish::BeginPlay()
{
	Super::BeginPlay();
}






void AOFK_ClownFish::PatrolProcess()
{
	//Super::PatrolProcess();

// 	FVector FloatingLocation;
// 
// 	float Xvalue = 3;
// 	float Yvalue = 1;
// 	float Zvalue = 4;
// 
// 	FVector FloatingValueVector;
// 	float SinValue = FMath::Sin(GetWorld()->DeltaTimeSeconds + FloatingTime) - FMath::Sin(FloatingTime);
// 	FloatingValueVector = FVector(Xvalue, Yvalue, Zvalue) * SinValue;
// 
// 	FloatingLocation = GetMesh()->GetRelativeLocation() + FloatingValueVector;
// 
// 	FloatingTime += GetWorld()->DeltaTimeSeconds;
// 
// 	GetMesh()->SetRelativeLocation(FloatingLocation);
}

void AOFK_ClownFish::AttackProcess()
{
	//Super::AttackProcess();
}

void AOFK_ClownFish::StunnedProcess()
{
	//Super::StunnedProcess();
}

void AOFK_ClownFish::ReturnProcess()
{
// 	// movement
// 	SplineMovementDistance = GetWorld()->DeltaTimeSeconds * Speed + SplineMovementDistance;
// 
// 	FTransform NewTransform;
// 
// 
// 	NewTransform = currentSpline->GetTransformAtDistanceAlongSpline(SplineMovementDistance, ESplineCoordinateSpace::Local);
// 	NewTransform.SetLocation(NewTransform.GetLocation() + DefaultPosition);
// 	GetMesh()->SetRelativeTransform(NewTransform);
// 
// 
// 	if (SplineMovementDistance >= currentSpline->GetSplineLength())
// 	{
// 		BehaviorState = EOceanFishBehaviorState::Patrol;
// 
// 		// initialize variables
// 		SplineMovementDistance = 0;
// 		//FloatingTime = 0;
// 
// 
// 
// 		OnMoving = false;
// 
// 	}
}



void AOFK_ClownFish::SetWebSocketArray()
{
	// null
}
