// Fill out your copyright notice in the Description page of Project Settings


#include "OFK_ClownFishFactory.h"
#include "OFK_ClownFish.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/ChildActorComponent.h>
#include <Components/SceneComponent.h>
#include <UObject/ConstructorHelpers.h>



AOFK_ClownFishFactory::AOFK_ClownFishFactory()
{
// 	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
// 	ChildActor->SetupAttachment(RootComponent);
// 
// 	ChildActor1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor1"));
// 	ChildActor1->SetupAttachment(RootComponent);
// 
// 	ChildActor2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor2"));
// 	ChildActor2->SetupAttachment(RootComponent);

	FishSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("FishSceneComp"));
	FishSceneComp->SetupAttachment(RootComponent);

	//ConstructorHelpers::FClassFinder<AOFK_ClownFish> FishBP(TEXT("/Game/BP/Character/BP_ClownFish1"));

	//if (FishBP.Succeeded())
	{
		for (int i = 0; i < FishCount; i++)
		{
			FString ActorName = "ClownFish" + FString::FromInt(i + 1);
			ClownFishArray.Add(CreateDefaultSubobject<UChildActorComponent>(FName(*ActorName)));
			ClownFishArray[i]->SetupAttachment(FishSceneComp);
			//ClownFishArray[i]->SetChildActorClass(FishBP.Class);

		}
	}


	/// <summary>
	/// 
	/// </summary>
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxFlySpeed = 30.f;
}

void AOFK_ClownFishFactory::BeginPlay()
{
	Super::BeginPlay();


	SetWebSocketArray();


	
}

void AOFK_ClownFishFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOFK_ClownFishFactory::SetWebSocketArray()
{

	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
	{
		webSocketInstance->OceanFishes[2] = this;
	}
}








void AOFK_ClownFishFactory::PatrolProcess()
{

}

void AOFK_ClownFishFactory::AttackProcess()
{

}

void AOFK_ClownFishFactory::StunnedProcess()
{

}

void AOFK_ClownFishFactory::ReturnProcess()
{

}



void AOFK_ClownFishFactory::SetAttackTargetLocation(const FVector location)
{

	for (auto element : ClownFishArray)
	{
		auto fish = Cast<AOFK_ClownFish>(element->GetChildActor());
		if (fish)
		{
			fish->InferredDirection = FactoryInferredDirection;
			fish->SetAttackTargetLocation(location);
			//fish->BehaviorState = EOceanFishBehaviorState::Attack;
			fish->OnMoving = true;
		}
	}

	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
	{
		if (webSocketInstance->bFishFeed)
		{
			auto Direcrtion = webSocketInstance->GetEnumerationToString(InferredDirection);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Direcrtion + "Feed Fish");
			UE_LOG(LogTemp, Warning, TEXT("Feed Fish"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Pronun");
		}
	}

	GetWorldTimerManager().ClearTimer(ReInferTimer);
	GetWorldTimerManager().SetTimer(ReInferTimer, this, &AOFK_ClownFishFactory::CheckChildMovingBool, 1.f, true, 1.f);





// 	auto temp = Cast<AOFK_ClownFish>(ChildActor->GetChildActor());
// 	if (temp)
// 	{
// 		//temp->AttackTargetLocation = location;
// 		//Super::SetAttackTargetLocation(location);
// 		temp->InferredDirection = FactoryInferredDirection;
// 		temp->BehaviorState = EOceanFishBehaviorState::Attack;
// 		temp->SetAttackTargetLocation(location);
// 		
// 		UE_LOG(LogTemp, Warning, TEXT("AA"));
// 	}


// 	auto temp1 = Cast<AOFK_ClownFish>(ChildActor1->GetChildActor());
// 	if (temp1)
// 	{
// 		temp1->InferredDirection = FactoryInferredDirection;
// 		temp1->BehaviorState = EOceanFishBehaviorState::Attack;
// 		temp1->SetAttackTargetLocation(location);
// 
// 		UE_LOG(LogTemp, Warning, TEXT("AA"));
// 	}
// 
// 	auto temp2 = Cast<AOFK_ClownFish>(ChildActor2->GetChildActor());
// 	if (temp2)
// 	{
// 		temp2->InferredDirection = FactoryInferredDirection;
// 		temp2->BehaviorState = EOceanFishBehaviorState::Attack;
// 		temp2->SetAttackTargetLocation(location);
// 
// 		UE_LOG(LogTemp, Warning, TEXT("AA"));
// 	}
}


void AOFK_ClownFishFactory::SetWebSocketQueueBool()
{
	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
	{
		webSocketInstance->OnInferDirectionClownFish = true;
		webSocketInstance->bFishFeed = false;
		webSocketInstance->ServerSendEndMessage("End", "Fish");
		//webSocketInstance->ServerSendMessage2("ID", webSocketInstance->WebSocketID);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No WebSocketInstance : cant reset OnInferDirectionFish bool"));

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Fish Timer Called");
	UE_LOG(LogTemp, Warning, TEXT("Fish Timer Called"));
}

void AOFK_ClownFishFactory::CheckChildMovingBool()
{
	/// <summary>
	/// 
	/// </summary>
	/// <param name="DeltaTime"></param>


	bool bChangeWebSocketBool = true;
	for (auto element : ClownFishArray)
	{
		auto fish = Cast<AOFK_ClownFish>(element->GetChildActor());
		if (fish)
		{
			if (fish->OnMoving == true)
			{
				bChangeWebSocketBool = false;
				//fishCount++;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Character is Die :: %s"), bChangeWebSocketBool? TEXT("true") : TEXT("false"));// 
	//UE_LOG(LogTemp, Log, TEXT("Character is Die :: %d"), fishCount);// 




	if (bChangeWebSocketBool == true)
	{
		// Timer
		FTimerHandle ResetQueueHandle;
		GetWorld()->GetTimerManager().SetTimer(ResetQueueHandle, this, &AOFK_ClownFishFactory::SetWebSocketQueueBool, ResetDelayTime, false, ResetDelayTime);
		GetWorldTimerManager().ClearTimer(ReInferTimer);
	}
}
