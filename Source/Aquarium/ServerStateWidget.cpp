// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerStateWidget.h"

void UServerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//SetColorAndText(FLinearColor::Red, "Server Connect Ready");

	ServerCommand(EServerState::None);

	// FTimerHandle TestHandle;
	// GetWorld()->GetTimerManager().SetTimer(TestHandle, FTimerDelegate::CreateLambda([&]()
	// {
	// 	ServerCommand(EServerState::Connect);
	// 	GetWorld()->GetTimerManager().ClearTimer(TestHandle);
	// }), 5.0f, false);
}

void UServerStateWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	GetWorld()->GetTimerManager().ClearTimer(BlankHandle);
}

void UServerStateWidget::SetColorAndText(FLinearColor Color, FString Msg)
{
	BlankImage->SetColorAndOpacity(Color);
	MessageTextBlock->SetText(FText::FromString(Msg));

	//GetWorld()->GetTimerManager().SetTimer(BlankHandle, this , &UServerStateWidget::BlankVisibillity, BlankSpeed, true);
}

void UServerStateWidget::BlankVisibillity()
{
	IsVisibillity = !IsVisibillity;

	//HideAndVisible(IsVisibillity);
}

void UServerStateWidget::HideAndVisible(bool isOn)
{
	if(BlankImage)
	{
		BlankImage->SetVisibility(isOn? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if(MessageTextBlock)
	{
		MessageTextBlock->SetVisibility(isOn? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UServerStateWidget::ServerCommand(EServerState State)
{
	switch (State)
	{
		default:
			break;
	case EServerState::None:
		{
			SetColorAndText(FLinearColor::Red, "Server Connect Ready...");

			GetWorld()->GetTimerManager().SetTimer(BlankHandle, this , &UServerStateWidget::BlankVisibillity, BlankSpeed, true);
		}
		break;
	case EServerState::Connect:
		{
			GetWorld()->GetTimerManager().ClearTimer(BlankHandle);
			
			FTimerHandle ConnectHandle;
			SetColorAndText(FLinearColor::Green, "Server Connect Complete");
	
			GetWorld()->GetTimerManager().SetTimer(ConnectHandle, FTimerDelegate::CreateLambda([&]()
			{
				HideAndVisible(false);
				GetWorld()->GetTimerManager().ClearTimer(ConnectHandle);
			}), 2.0f, false);
		}
		break;
	case EServerState::ReConnecting:
		{
			SetColorAndText(FLinearColor::Red, "Reconnecting Server...");

			GetWorld()->GetTimerManager().SetTimer(BlankHandle, this , &UServerStateWidget::BlankVisibillity, BlankSpeed, true);
		}
		break;
	case EServerState::DisConnect:
		{
			SetColorAndText(FLinearColor::Red, "Server Off Signal Check Your Server");

			GetWorld()->GetTimerManager().SetTimer(BlankHandle, this , &UServerStateWidget::BlankVisibillity, BlankSpeed, true);
		}
		break;
	}
	
	
	
}

void UServerStateWidget::OnServerConnectComplete()
{
	
}


