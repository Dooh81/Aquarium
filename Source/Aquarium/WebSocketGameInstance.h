// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "Containers/CircularQueue.h"

#include "WebSocketGameInstance.generated.h"


DECLARE_DELEGATE_TwoParams(FEventDelegate, EInferredDir, int);
/**
 * 
 */

UENUM(BlueprintType)
enum class EInferredDir :uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	Center UMETA(DisplayName = "Center"),
	None UMETA(DisplayName = "None"), 
};


USTRUCT(BlueprintType)
struct FReceivedData
{
	GENERATED_BODY()

public:
	EInferredDir	Label = EInferredDir::None;
	uint8			Confidence = 0;
	FString			Event = "";
	FString			InteractionMode = "Pronun";
};


USTRUCT(BlueprintType)
struct FNumDir
{
	GENERATED_BODY()

public:
	uint8 LeftNum = 0;
	uint8 RightNum = 0;
	uint8 CenterNum = 0;
};




UCLASS()
class AQUARIUM_API UWebSocketGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:

// 	virtual void BeginPlay() override;


public:
	virtual void Init() override;
	virtual void Shutdown() override;

	TSharedPtr<IWebSocket> WebSocket;
	//FVector2D SendLocation;

	void OnReceived(const FString& MessageString);
	void OnReConnectionServer(int32 Num, const FString& MessageString, bool isBool);
	void OnConnectedEvent();
	void OnConnectionErrorEvent(const FString& Msg);
	void ReConnectedServer();


	FTimerHandle ServerDisconnectedHandle;

	int32 ServerRepeats = 0;

	bool bDevMode;

public:

	// fish array 에 tuple로 넣어야 할듯?
	bool OnInferDirectionShark = true;
	bool OnInferDirectionKids = true;
	bool OnInferDirectionClownFish = true;

	int32 DataMaxNum = 10;
 	//TCircularQueue<FReceivedData> ReceivedDataQueue = TCircularQueue<FReceivedData>(DataMaxNum - 1 );
 	TCircularQueue<FReceivedData> SharkReceivedDataQueue = TCircularQueue<FReceivedData>(DataMaxNum - 1 );
	TCircularQueue<FReceivedData> KidsReceivedDataQueue = TCircularQueue<FReceivedData>(DataMaxNum - 1 );
	TCircularQueue<FReceivedData> ClownFishReceivedDataQueue = TCircularQueue<FReceivedData>(DataMaxNum - 1 );



 	FNumDir SharkNumDir;
	FNumDir KidsNumDir;
	FNumDir ClownFishNumDir;

	FString SharkWebSocketID = "";
	FString TurtleWebSocketID = "";
	FString FishWebSocketID = "";
	//


	// feed
	bool bSharkFeed = false;
	bool bTurtleFeed = false;
	bool bFishFeed = false;

	EInferredDir SharkFeedDir;
	EInferredDir TurtleFeedDir;
	EInferredDir FishFeedDir;




	uint8 ConfidenceRef = 40;

private:
 	void EnqueueNumDir(FNumDir& NumDirStruct, EInferredDir Dir);
 	void DequeueNumDir(FNumDir& NumDirStruct, EInferredDir Dir);
	EInferredDir GetInferredDirection(FNumDir& NumDirStruct);
	int GetMaxCount(FNumDir& NumDirStruct);


 	void QueueProcess(TCircularQueue<FReceivedData>& ReceivedDataQueue, FReceivedData tempData, FNumDir& NumDir);

	
public:
	FEventDelegate EventDelegateOneParam;


public:
	UPROPERTY()
		TArray<class AOceanFish*> OceanFishes = {nullptr, nullptr, nullptr};	//0 = shark , 1 = turtle, 2 = clownfish

	void ServerSendEventMessage(FString Division, EInferredDir Direction);
	void ServerSendEndMessage(FString End, FString FishCategory);

	UFUNCTION(BlueprintCallable)
	void ConnectWS();

	template <typename Enumeration>
	static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
	{
		return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue)).Mid(0);
	}
};

