// Fill out your copyright notice in the Description page of Project Settings.


#include "OFK_Anim.h"

#include <Animation/AnimMontage.h>
#include <Components/SplineComponent.h>


void AOFK_Anim::BeginPlay()
{
	Super::BeginPlay();

		
//  	AnimMontage1 = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Anim/turtle/left/VA_ani_tutle_v003_left_pub_Anim_root_Action_Montage"));
//  	AnimMontage2 = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Anim/turtle/mid/VA_ani_tutle_v003_mid_pub_Anim_root_002_Action_Montage"));
//  	AnimMontage3 = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Anim/turtle/right/VA_ani_tutle_v003_right_pub_Anim_root_001_Action1_Montage"));
// 
	auto SkMesh = GetMesh();
	GetMesh()->SetVisibility(false);
}

void AOFK_Anim::PatrolProcess()
{
}

void AOFK_Anim::AttackProcess()
{
}

void AOFK_Anim::StunnedProcess()
{
}

void AOFK_Anim::ReturnProcess()
{
	GetMesh()->SetVisibility(false);



	//timer
	// websocket queue bool
	SetWebSocketQueueBool();

	BehaviorState = EOceanFishBehaviorState::Patrol;
}





void AOFK_Anim::SetAttackTargetLocation(const FVector location)
{
	AttackTargetLocation = location;
	BehaviorState = EOceanFishBehaviorState::Attack;
	auto SkMesh = GetMesh();
	GetMesh()->SetVisibility(true);
	FVector Location;
	FRotator Rotation;

	switch (InferredDirection)
	{
	case EInferredDir::Left:

		UE_LOG(LogTemp, Warning, TEXT("Left"));

		Location = LeftSplineComp->GetComponentLocation();
		Rotation = LeftSplineComp->GetRelativeRotation();
		SkMesh->SetWorldLocation(Location);
		SkMesh->SetRelativeRotation(Rotation);
		SkMesh->PlayAnimation(AnimMontage1, false);
		SkMesh->SetPlayRate(LeftSpeed);

		break;

	case EInferredDir::Center:
		UE_LOG(LogTemp, Warning, TEXT("Center"));

		Location = CenterSplineComp->GetComponentLocation();
		Rotation = CenterSplineComp->GetRelativeRotation();
		SkMesh->SetWorldLocation(Location);
		SkMesh->SetRelativeRotation(Rotation);
		SkMesh->PlayAnimation(AnimMontage2, false);
		SkMesh->SetPlayRate(CenterSpeed);

		break;

	case EInferredDir::Right:
		UE_LOG(LogTemp, Warning, TEXT("Right"));

		Location = RightSplineComp->GetComponentLocation();
		Rotation = RightSplineComp->GetRelativeRotation();
		SkMesh->SetWorldLocation(Location);
		SkMesh->SetRelativeRotation(Rotation);
		SkMesh->PlayAnimation(AnimMontage3, false);
		SkMesh->SetPlayRate(RightSpeed);
		
		break;


	default:

		break;

	}


}

void AOFK_Anim::SetStateReturn(const EOceanFishBehaviorState value)
{
 	GetMesh()->SetVisibility(true);
	BehaviorState = value;

}

