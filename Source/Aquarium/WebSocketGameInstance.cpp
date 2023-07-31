// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketGameInstance.h"

#include "PlayerPawn.h"
#include "WebSocketsModule.h"
#include "Kismet/GameplayStatics.h"
#include "OceanFish.h"

#include <vector>
#include <algorithm>

#include "AquaGameMode.h"
#include "AquaHUD.h"
#include "GameFramework/GameModeBase.h"


void UWebSocketGameInstance::Init()
{
	Super::Init();
}


void UWebSocketGameInstance::Shutdown()
{
	if (bDevMode)
	{
		if (WebSocket->IsConnected())
		{
			WebSocket->Close();
		}
	}

	Super::Shutdown();
}

void UWebSocketGameInstance::OnReceived(const FString& MessageString)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Received message: " + MessageString);

	const FString Data = MessageString;


	// json �����ͷ� ��ȯ
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(Data.Mid(1, Data.Len() - 2));
	//TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(Data); 
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	FString tempLabel = "";
	int32 tempConfidence = 0;
	FString tempEvent = "";
	FString WebSocketID = "";
	FString InteractionMode = "";
	FString Shake = "";


	if (FJsonSerializer::Deserialize(reader, jsonObj) && jsonObj.IsValid())
	{
		jsonObj->TryGetStringField("label", tempLabel);
		jsonObj->TryGetNumberField("confidence", tempConfidence);
		jsonObj->TryGetStringField("Event", tempEvent);
		jsonObj->TryGetStringField("ID", WebSocketID);
		jsonObj->TryGetStringField("Mode", InteractionMode);
		jsonObj->TryGetStringField("Shake", Shake);
	}


	if (OnInferDirectionShark || OnInferDirectionClownFish || OnInferDirectionKids)
	{
		// Struct: Create tempData
		FReceivedData tempData;
		if (tempLabel == "Left")
			tempData.Label = EInferredDir::Left;
		else if (tempLabel == "Right")
			tempData.Label = EInferredDir::Right;
		else if (tempLabel == "Center")
			tempData.Label = EInferredDir::Center;


		tempData.Confidence = tempConfidence;
		tempData.Event = tempEvent;
		tempData.InteractionMode = InteractionMode;

		// Execute Each Queue
		if (tempData.Event == "Shark" && OnInferDirectionShark == true)
		{
			if (bSharkFeed)
			{
				if (Shake == "Shake")
				{
					EventDelegateOneParam.Execute(SharkFeedDir, 0);
					OnInferDirectionShark = false;
				}
				return;
			}

			SharkWebSocketID = WebSocketID;
			QueueProcess(SharkReceivedDataQueue, tempData, SharkNumDir);
		}
		else if (tempData.Event == "Fish" && OnInferDirectionClownFish == true)
		{
			if (bFishFeed)
			{
				if (Shake == "Shake")
				{
					EventDelegateOneParam.Execute(FishFeedDir, 2);
					OnInferDirectionClownFish = false;
				}
				return;
			}

			FishWebSocketID = WebSocketID;
			QueueProcess(ClownFishReceivedDataQueue, tempData, ClownFishNumDir);
		}
		else if (tempData.Event == "Turtle" && OnInferDirectionKids == true)
		{
			if (bTurtleFeed)
			{
				if (Shake == "Shake")
				{
					EventDelegateOneParam.Execute(TurtleFeedDir, 1);
					OnInferDirectionKids = false;
				}
					return;
			}


			TurtleWebSocketID = WebSocketID;
			QueueProcess(KidsReceivedDataQueue, tempData, KidsNumDir);
		}
		else
			return;

		//QueueProcess(SharkReceivedDataQueue, tempData);
	}
}

void UWebSocketGameInstance::EnqueueNumDir(FNumDir& NumDirStruct, EInferredDir Dir)
{
	switch (Dir)
	{
	case EInferredDir::Left:
		NumDirStruct.LeftNum++;
		break;

	case EInferredDir::Right:
		NumDirStruct.RightNum++;
		break;

	case EInferredDir::Center:
		NumDirStruct.CenterNum++;
		break;
	}
}

void UWebSocketGameInstance::DequeueNumDir(FNumDir& NumDirStruct, EInferredDir Dir)
{
	switch (Dir)
	{
	case EInferredDir::Left:
		NumDirStruct.LeftNum--;
		break;

	case EInferredDir::Right:
		NumDirStruct.RightNum--;
		break;

	case EInferredDir::Center:
		NumDirStruct.CenterNum--;
		break;
	}
}

EInferredDir UWebSocketGameInstance::GetInferredDirection(FNumDir& NumDirStruct)
{
	std::vector<uint8> v;
	v.push_back(NumDirStruct.LeftNum);
	v.push_back(NumDirStruct.RightNum);
	v.push_back(NumDirStruct.CenterNum);

	int max_index = max_element(v.begin(), v.end()) - v.begin();

	switch (max_index)
	{
	case 0:
		return EInferredDir::Left;
		break;

	case 1:
		return EInferredDir::Right;
		break;

	case 2:
		return EInferredDir::Center;
		break;

	default:
		return EInferredDir::None;
		break;
	}
}

int UWebSocketGameInstance::GetMaxCount(FNumDir& NumDirStruct)
{
	std::vector<uint8> v;
	v.push_back(NumDirStruct.LeftNum);
	v.push_back(NumDirStruct.RightNum);
	v.push_back(NumDirStruct.CenterNum);

	int max = *max_element(v.begin(), v.end());

	return max;
}

void UWebSocketGameInstance::QueueProcess(TCircularQueue<FReceivedData>& ReceivedDataQueue, FReceivedData tempData,
                                          FNumDir& NumDir)
{
	// ���� confidence �̻� ť�� ����
	if (tempData.Confidence >= ConfidenceRef)
	{
#pragma region Circular Queue

		if (ReceivedDataQueue.IsFull() == false) // Enqueue til CircularQueue be full
		{
			// queue ����
			ReceivedDataQueue.Enqueue(tempData);
			// DirCount
			EnqueueNumDir(NumDir, tempData.Label);
		}
		else //if (ReceivedDataQueue.IsFull())
		{
			// ����
			auto removeData = ReceivedDataQueue.Peek();
			DequeueNumDir(NumDir, removeData->Label);
			ReceivedDataQueue.Dequeue();

			// ����
			ReceivedDataQueue.Enqueue(tempData);
			EnqueueNumDir(NumDir, tempData.Label);


			int maxValue = GetMaxCount(NumDir);
			float Percentage = (maxValue / ReceivedDataQueue.Count()) * 100;
			if (Percentage >= 60)
			{
				// screen debug
				auto InferredDir = GetInferredDirection(NumDir);
				const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInferredDir"), true);
				FString tempDirString = EnumPtr->GetDisplayNameTextByValue((int64)InferredDir).ToString();
				auto maxValueString = FString::FromInt(maxValue);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
				                                 "Attack to " + tempDirString + "  " + maxValueString);


				// Execute Each Queue
				// call delegate
				if (EventDelegateOneParam.IsBound() == true)
				{
					if (tempData.Event == "Shark")
					{

						if (tempData.InteractionMode == "Pronun")
						{
							OceanFishes[0]->InteractionMode = EInteractionMode::Pronun;
							EventDelegateOneParam.Execute(InferredDir, 0);
							OnInferDirectionShark = false;
						}
						if (tempData.InteractionMode == "Feeding")
						{
							bSharkFeed = true;
							OceanFishes[0]->InteractionMode = EInteractionMode::Feeding;
							SharkFeedDir = InferredDir;
						}
					}
					else if (tempData.Event == "Turtle")
					{

						if (tempData.InteractionMode == "Pronun")
						{
							OceanFishes[1]->InteractionMode = EInteractionMode::Pronun;
							EventDelegateOneParam.Execute(InferredDir, 1);
							OnInferDirectionKids = false;
						}
						if (tempData.InteractionMode == "Feeding")
						{
							bTurtleFeed = true;
							OceanFishes[1]->InteractionMode = EInteractionMode::Feeding;
							TurtleFeedDir = InferredDir;
						}
					}
					else if (tempData.Event == "Fish")
					{

						if (tempData.InteractionMode == "Pronun")
						{
							OceanFishes[2]->InteractionMode = EInteractionMode::Pronun;
							EventDelegateOneParam.Execute(InferredDir, 2);
							OnInferDirectionClownFish = false;
						}
						if (tempData.InteractionMode == "Feeding")
						{
							bFishFeed = true;
							OceanFishes[2]->InteractionMode = EInteractionMode::Feeding;
							FishFeedDir = InferredDir;
						}
					}


					ServerSendEventMessage(tempData.Event, InferredDir);
				}

				// queue �ʱ�ȭ
				while (!ReceivedDataQueue.IsEmpty())
				{
					ReceivedDataQueue.Dequeue();

					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, "On Dequeue !!!");
				}

				// Dir Count �ʱ�ȭ
				if (ReceivedDataQueue.IsEmpty())
				{
					NumDir.LeftNum = 0;
					NumDir.RightNum = 0;
					NumDir.CenterNum = 0;
				}


				// Execute Each Queue
				// 				if (tempData.Event == "Shark")
				// 					OnInferDirectionShark = false;
				// 				else if (tempData.Event == "Fish")
				// 					OnInferDirectionClownFish = false;
				// 				else if (tempData.Event == "Turtle")
				// 					OnInferDirectionKids = false;
			}
		}
#pragma endregion


#pragma region queue
		// 			// Queue ( length = x '30' ) ���� ������ �ޱ�
		// 			if (ReceivedDataQueue.Count() < DataMaxNum)		// queue ���̰� maxNum ���� ������ 
		// 			{
		// 				// queue ����
		// 				ReceivedDataQueue.Enqueue(tempData);
		// 				EnqueueNumDir(NumDir, tempData.Label);
		// 				// NumDir
		// 				
		// 			}
		// 			else if (ReceivedDataQueue.Count() > DataMaxNum)	// queue ���̰� maxNum ���� Ŭ ��
		// 			{
		// 				// �������� ���� ����
		// 				while(ReceivedDataQueue.Count() != DataMaxNum)
		// 				{
		// 					auto removeData = ReceivedDataQueue.Peek();
		// 					DequeueNumDir(NumDir, removeData->Label);
		// 					ReceivedDataQueue.Dequeue();
		// 				}
		// 			}
		// 			else if (ReceivedDataQueue.Count() == DataMaxNum)	// queue ���̰� maxNum �� ���� ��
		// 			{
		// 				// ����
		// 				auto removeData = ReceivedDataQueue.Peek();
		// 				DequeueNumDir(NumDir, removeData->Label);
		// 				ReceivedDataQueue.Dequeue();
		// 
		// 				// ����
		// 				ReceivedDataQueue.Enqueue(tempData);
		// 				EnqueueNumDir(NumDir, tempData.Label);
		// 
		// 
		// 
		// 			
		// 			
		// 			
		// 
		// 
		// 				int maxValue = GetMaxCount(NumDir);
		// 				float Percentage = (maxValue / ReceivedDataQueue.Count()) * 100;
		// 				if (Percentage >= 60)
		// 				{
		// 					// screen debug
		// 					auto InferredDir = GetInferredDirection(NumDir);
		// 					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInferredDir"), true);
		// 					FString tempDirString = EnumPtr->GetDisplayNameTextByValue((int64)InferredDir).ToString();
		// 					auto maxValueString = FString::FromInt(maxValue);
		// 					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Attack to " + tempDirString + "  " + maxValueString);
		// 
		// 
		// 					// queue �ʱ�ȭ
		// 					while(!ReceivedDataQueue.IsEmpty())
		// 					{
		// 						ReceivedDataQueue.Dequeue();
		// 
		// 						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "On Dequeue !!!");
		// 
		// 					}
		// 
		// 					// Dir Count �ʱ�ȭ
		// 					if (ReceivedDataQueue.IsEmpty())
		// 					{			
		// 						NumDir.LeftNum = 0;
		// 						NumDir.RightNum = 0;
		// 						NumDir.CenterNum = 0;
		// 					}
		// 
		// 				}
		// 			}
#pragma endregion
	}


	// screen debug test
	if (!ReceivedDataQueue.IsEmpty())
	{
		FString tempQueueLength = FString::FromInt(ReceivedDataQueue.Count());
		auto tempPeekData = ReceivedDataQueue.Peek();
		FString tempPeekDataConfidence = FString::FromInt(tempPeekData->Confidence);
		//		FString::FromInt(NumDir.LeftNum);
		// 		
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 "Queue Length: " + tempQueueLength + "    LastDataConfidence: " +
		                                 tempPeekDataConfidence);
	}
}

void UWebSocketGameInstance::ServerSendEventMessage(FString Division, EInferredDir Direction)
{
	FString DirectionValue = GetEnumerationToString<EInferredDir>(Direction);

	UE_LOG(LogTemp, Warning, TEXT("Name: %s,   Direction: %s"), *Division, *DirectionValue);


	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

	RequestObj->SetStringField(DirectionValue, Division);
	if (Division == "Shark")
		RequestObj->SetStringField("ID", SharkWebSocketID);
	else if (Division == "Turtle")
		RequestObj->SetStringField("ID", TurtleWebSocketID);
	else if (Division == "Fish")
		RequestObj->SetStringField("ID", FishWebSocketID);


	if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
	{
		if (!bDevMode) return;

		WebSocket->Send(RequestBody);
	}
}

void UWebSocketGameInstance::ServerSendEndMessage(FString End, FString FishCategory)
{
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

	RequestObj->SetStringField(End, FishCategory);
	if (FishCategory == "Shark")
		RequestObj->SetStringField("ID", SharkWebSocketID);
	else if (FishCategory == "Turtle")
		RequestObj->SetStringField("ID", TurtleWebSocketID);
	else if (FishCategory == "Fish")
		RequestObj->SetStringField("ID", FishWebSocketID);


	if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
	{
		if (!bDevMode) return;

		WebSocket->Send(RequestBody);
	}
}

void UWebSocketGameInstance::ConnectWS()
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket("ws://10.12.114.22:8080");

	WebSocket->OnConnected().AddUObject(this, &UWebSocketGameInstance::OnConnectedEvent);

	WebSocket->OnConnectionError().AddUObject(this, &UWebSocketGameInstance::OnConnectionErrorEvent);

	WebSocket->OnClosed().AddUObject(this, &UWebSocketGameInstance::OnReConnectionServer);


	WebSocket->OnMessage().AddUObject(this, &UWebSocketGameInstance::OnReceived);


	WebSocket->OnMessage().AddLambda([](const FString& MessageString)
	{
	});

	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Sent message: " + MessageString);
	});

	WebSocket->Connect();

	bDevMode = true;
}

void UWebSocketGameInstance::OnReConnectionServer(int32 Num, const FString& MessageString, bool isBool)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Connection closed ");


	GetTimerManager().SetTimer(ServerDisconnectedHandle, this, &UWebSocketGameInstance::ReConnectedServer, 5.0f, true);
}

void UWebSocketGameInstance::OnConnectedEvent()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Successfully connected");

	AAquaHUD* AquaHUD = Cast<AAquaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	AquaHUD->ServerMessage(EServerState::Connect);
}

void UWebSocketGameInstance::OnConnectionErrorEvent(const FString& Msg)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Msg);

	AAquaHUD* AquaHUD = Cast<AAquaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	AquaHUD->ServerMessage(EServerState::DisConnect);
}

void UWebSocketGameInstance::ReConnectedServer()
{
	if (WebSocket->IsConnected())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Reconnect Complete Server.");
		GetTimerManager().ClearTimer(ServerDisconnectedHandle);

		AAquaHUD* AquaHUD = Cast<AAquaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		AquaHUD->ServerMessage(EServerState::Connect);
		return;
	}
	else
	{
		AAquaHUD* AquaHUD = Cast<AAquaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		AquaHUD->ServerMessage(EServerState::ReConnecting);
	}

	if (ServerRepeats >= 5)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Check Your Server.");
		GetTimerManager().ClearTimer(ServerDisconnectedHandle);
		return;
	}

	ServerRepeats++;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Reconnecting...");
	WebSocket->Connect();

	bDevMode = true;
}
