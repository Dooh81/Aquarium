// Fill out your copyright notice in the Description page of Project Settings.


#include "AquaHUD.h"

#include "Blueprint/UserWidget.h"

void AAquaHUD::BeginPlay()
{
	Super::BeginPlay();

	if(WidgetClass != nullptr)
	{
		UUserWidget* AquaWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

		if(AquaWidget != nullptr)
		{
			AquaWidget->AddToViewport();

			ServerStateWidget = Cast<UServerStateWidget>(AquaWidget);
		}
	}
}

void AAquaHUD::ServerMessage(EServerState State)
{
	ServerStateWidget->ServerCommand(State);
}
