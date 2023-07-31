// Fill out your copyright notice in the Description page of Project Settings.


#include "OFS_SharkAnim.h"

#include "Components/SkeletalMeshComponent.h"

AOFS_SharkAnim::AOFS_SharkAnim()
{

	FeedCenter1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feed1"));
	FeedCenter1->SetupAttachment(RootComponent);
}

void AOFS_SharkAnim::BeginPlay()
{
	Super::BeginPlay();

	auto webSocketInstance = Cast<UWebSocketGameInstance>(GetWorld()->GetGameInstance());
	if (webSocketInstance)
		webSocketInstance->OceanFishes[0] = this;

}



void AOFS_SharkAnim::PatrolProcess()
{

}

void AOFS_SharkAnim::AttackProcess()
{

}

void AOFS_SharkAnim::StunnedProcess()
{

}

void AOFS_SharkAnim::ReturnProcess()
{
	InitInteractionBool();

	ResetWebSocketBool();


	BehaviorState = EOceanFishBehaviorState::Patrol;
}





void AOFS_SharkAnim::InitInteractionBool()
{
	bFeedLeft = false;
	bFeedCenter = false;
	bFeedRight = false;

	bPronunLeft = false;
	bPronunCenter = false;
	bPronunRight = false;

	UE_LOG(LogTemp, Warning, TEXT("Init Bools"));

}

void AOFS_SharkAnim::PlayFeedAnim()
{
	FeedCenter1->PlayAnimation(CurrentAnim, false);
}

void AOFS_SharkAnim::SetAttackTargetLocation(const FVector location)
{
	AttackTargetLocation = location;

	BehaviorState = EOceanFishBehaviorState::Attack;

	switch (InteractionMode)
	{
		case EInteractionMode::Feeding:
		{
			switch (InferredDirection)
			{
			case EInferredDir::Left:
			{
				UE_LOG(LogTemp, Warning, TEXT("Feed Left"));

				if (FeedCenterAnimMontage1)
				{
					CurrentAnim = FeedCenterAnimMontage1;
					FeedCenter1->SetVisibility(true);

				}


				bFeedLeft = true;
			}
			break;

			case EInferredDir::Center:
			{
				UE_LOG(LogTemp, Warning, TEXT("Feed Center"));

				if(FeedCenterAnimMontage1)
				{
					CurrentAnim = FeedCenterAnimMontage1;
					FeedCenter1->SetVisibility(true);

				}


				bFeedCenter = true;
			}
			break;

			case EInferredDir::Right:
			{
				UE_LOG(LogTemp, Warning, TEXT("Feed Right"));

				if (FeedCenterAnimMontage1)
				{
					CurrentAnim = FeedCenterAnimMontage1;
					FeedCenter1->SetVisibility(true);

				}


				bFeedRight = true;
			}
			break;

			default:

				break;
			}
		}
		break;


		case EInteractionMode::Pronun:
		{
			// temp
			BehaviorState = EOceanFishBehaviorState::Return;

			switch (InferredDirection)
			{
			case EInferredDir::Left:
			{
				UE_LOG(LogTemp, Warning, TEXT("Pronon Left"));

				bPronunLeft = true;
			}
			break;

			case EInferredDir::Center:
			{
				UE_LOG(LogTemp, Warning, TEXT("Pronon Center"));

				bPronunCenter = true;
			}
			break;

			case EInferredDir::Right:
			{
				UE_LOG(LogTemp, Warning, TEXT("Pronon Right"));

				bPronunRight = true;
			}
			break;

			default:

				break;
			}
		}
		break;
	}

}

void AOFS_SharkAnim::SetStateReturn(const EOceanFishBehaviorState value)
{
	BehaviorState = value;

	FeedCenter1->SetVisibility(false);

	UE_LOG(LogTemp, Warning, TEXT("SetStateReturn"));

}
