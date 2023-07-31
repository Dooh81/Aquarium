// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerStateWidget.h"
#include "GameFramework/HUD.h"
#include "AquaHUD.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUM_API AAquaHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerStateWidget> WidgetClass;

	UServerStateWidget* ServerStateWidget;

	virtual void BeginPlay() override;

public:
	void ServerMessage(EServerState State);
};
