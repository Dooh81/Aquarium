// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ServerStateWidget.generated.h"

UENUM(BlueprintType)
enum class EServerState :uint8
{
	None UMETA(DisplayName = "None"), 
	Connect UMETA(DisplayName = "Connect"),
	ReConnecting UMETA(DisplayName = "ReConnecting"),
	DisConnect UMETA(DisplayName = "DisConnect"),
};

UCLASS()
class AQUARIUM_API UServerStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta=(BindWidget))
	UImage* BlankImage;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MessageTextBlock;

	FTimerHandle BlankHandle;


	bool IsVisibillity = false;

	float BlankSpeed = 1.0f;
	
	UFUNCTION()
	void SetColorAndText(FLinearColor Color, FString Msg);

	UFUNCTION()
	void BlankVisibillity();

	UFUNCTION()
	void HideAndVisible(bool isOn);

	UFUNCTION()
	void ServerCommand(EServerState State);

	UFUNCTION()
	void OnServerConnectComplete();
};
