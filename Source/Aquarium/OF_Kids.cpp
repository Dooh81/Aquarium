// Fill out your copyright notice in the Description page of Project Settings.


#include "OF_Kids.h"
#include "Components/SplineComponent.h"
#include "Components/SkeletalmeshComponent.h"
#include <GameFramework/Character.h>
#include "WebSocketGameInstance.h"
#include <GameFramework/CharacterMovementComponent.h>


AOF_Kids::AOF_Kids()
{
	/// <summary>
	/// In
	/// </summary>
	LeftSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("LeftSplineComp"));
	LeftSplineComp->SetupAttachment(RootComponent);

	RightSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("RightSplineComp"));
	RightSplineComp->SetupAttachment(RootComponent);

	CenterSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("CenterSplineComp"));
	CenterSplineComp->SetupAttachment(RootComponent);



	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxFlySpeed = 100.f;

	/// <summary>
	/// Out
	/// </summary>
// 	LeftAwaySplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("LeftAwaySplineComp"));
// 	LeftAwaySplineComp->SetupAttachment(RootComponent);
// 
// 	RightAwaySplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("RightAwaySplineComp"));
// 	RightAwaySplineComp->SetupAttachment(RootComponent);
// 
// 	CenterAwaySplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("CenterAwaySplineComp"));
// 	CenterAwaySplineComp->SetupAttachment(RootComponent);


	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxFlySpeed = 100.f;
}





void AOF_Kids::BeginPlay()
{
	Super::BeginPlay();


	SetWebSocketArray();
}



// movement
void AOF_Kids::PatrolProcess()
{
	GetMesh()->SetVisibility(false);

}

void AOF_Kids::AttackProcess()
{

	//return;
	if(currentSpline == nullptr)
		return;




	//if (SplineMovementDistance >= currentSpline->GetSplineLength())
	if (SplineMovementDistance >= currentSpline->GetDistanceAlongSplineAtSplineInputKey(CurrentStunnedIndex))
	{
		BehaviorState = EOceanFishBehaviorState::Stunned;

		// Timer : Stunned -> Return
		FTimerHandle StunTimerHandle;
		GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AOF_Kids::SetReturnState, 1, false, 5);

		return;
	}



	// movement
	SplineMovementDistance = GetWorld()->DeltaTimeSeconds * Speed + SplineMovementDistance;

	FTransform NewTransform;


	NewTransform = currentSpline->GetTransformAtDistanceAlongSpline(SplineMovementDistance, ESplineCoordinateSpace::Local);
	NewTransform.SetLocation(NewTransform.GetLocation() + DefaultPosition);
	GetMesh()->SetRelativeTransform(NewTransform);






}

void AOF_Kids::StunnedProcess()
{
	FVector FloatingLocation;

	float Xvalue = 2;
	float Yvalue = 1;
	float Zvalue = 2;

	FVector FloatingValueVector;
	float SinValue = FMath::Sin(GetWorld()->DeltaTimeSeconds + FloatingTime) - FMath::Sin(FloatingTime);
	FloatingValueVector = FVector(Xvalue, Yvalue, Zvalue) * SinValue;

	FloatingLocation = GetMesh()->GetRelativeLocation() + FloatingValueVector ;

	FloatingTime += GetWorld()->DeltaTimeSeconds;
		
	GetMesh()->SetRelativeLocation(FloatingLocation);



}

void AOF_Kids::ReturnProcess()
{
// 	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
// 	if (webSocketInstance)
// 	{
// 		webSocketInstance->OnInferDirection = true;
// 	}


	// movement
	SplineMovementDistance = GetWorld()->DeltaTimeSeconds * Speed + SplineMovementDistance;

	FTransform NewTransform;


	NewTransform = currentSpline->GetTransformAtDistanceAlongSpline(SplineMovementDistance, ESplineCoordinateSpace::Local);
	NewTransform.SetLocation(NewTransform.GetLocation() + DefaultPosition);
	GetMesh()->SetRelativeTransform(NewTransform);


	if (SplineMovementDistance >= currentSpline->GetSplineLength())
	{
		BehaviorState = EOceanFishBehaviorState::Patrol;

		// initialize variables
		SplineMovementDistance = 0;
		FloatingTime = 0;





		// websocket queue bool
		FTimerHandle ResetQueueHandle;
		GetWorld()->GetTimerManager().SetTimer(ResetQueueHandle, this, &AOF_Kids::SetWebSocketQueueBool, ResetDelayTime, false, ResetDelayTime);



// 		// Timer : Stunned -> Return
// 		FTimerHandle StunTimerHandle;
// 		GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AOF_Kids::SetReturnState, 1, false, 5);
	}
}





////////////////////////////////////////

void AOF_Kids::SetReturnState()
{
	BehaviorState = EOceanFishBehaviorState::Return;
}

///////////////////////////////////////





void AOF_Kids::SetAttackTargetLocation(const FVector location)
{
	Super::SetAttackTargetLocation(location);

	GetMesh()->SetVisibility(true);
	FVector ZeroIndexPosition;

	switch (InferredDirection)
	{
		case EInferredDir::Left:
			currentSpline = LeftSplineComp;
			CurrentStunnedIndex = LeftStunnedIndex;

			break;

		case EInferredDir::Right:
			currentSpline = RightSplineComp;
			CurrentStunnedIndex = RightStunnedIndex;

			break;

		case EInferredDir::Center:
			currentSpline = CenterSplineComp;
			CurrentStunnedIndex = CenterStunnedIndex;

			break;

		default:
			currentSpline = RightSplineComp;
			CurrentStunnedIndex = RightStunnedIndex;

			break;

	}

	ZeroIndexPosition = currentSpline->GetWorldLocationAtSplinePoint(0);
	GetMesh()->SetWorldLocation(ZeroIndexPosition);
	DefaultPosition = GetMesh()->GetRelativeLocation();



	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
	{
		if (webSocketInstance->bTurtleFeed)
		{
			auto Direcrtion = webSocketInstance->GetEnumerationToString(InferredDirection);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Direcrtion + "Feed Turtle");

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Pronun");
		}
	}
}







void AOF_Kids::SetWebSocketArray()
{
	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
		webSocketInstance->OceanFishes[1] = this;
}

void AOF_Kids::SetWebSocketQueueBool()
{
	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
	{
		webSocketInstance->OnInferDirectionKids = true;
		webSocketInstance->bTurtleFeed = false;
		webSocketInstance->ServerSendEndMessage("End", "Turtle");
		//webSocketInstance->ServerSendMessage2("ID", webSocketInstance->WebSocketID);

	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No WebSocketInstance : cant reset OnInferDirectionKids bool"));

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Kids Timer Called");
	UE_LOG(LogTemp, Warning, TEXT("Kids Timer Called"));

}
