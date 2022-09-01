// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/FindedSessionWidget.h"

#include "Components/TextBlock.h"
#include "Player/MenuPlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UserWidget/SessionUserWidget.h"


void UFindedSessionWidget::TryJoinSession()
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid())
	{
		return;
	}
	SessionPtr->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UFindedSessionWidget::OnJoinSessionCompleted));

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	SessionPtr->JoinSession(*localPlayer->GetPreferredUniqueNetId(), FName("SessionName"), SessionResult);
}


void UFindedSessionWidget::SetupWidgetSettings()
{
	FString MapName;
	checkf(SessionResult.Session.SessionSettings.Get(SETTING_GAMEMODE, RoomName), TEXT("FindedSession had wrong session FName!"));
	checkf(SessionResult.Session.SessionSettings.Get(SETTING_MAPNAME, MapName), TEXT("FindedSession had wrong map FName!"));

	FText Players = FText::FromString(FString::FromInt(SessionResult.Session.NumOpenPrivateConnections) + "/"	
										+ FString::FromInt(SessionResult.Session.NumOpenPublicConnections));

	PingBlock->SetText(FText::AsNumber(SessionResult.PingInMs));
	CurrentPlayersBlock->SetText(Players);
	ServerNameBlock->SetText(FText::FromString(RoomName));
	MapNameBlock->SetText(FText::FromString(MapName));
}

void UFindedSessionWidget::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::FromInt(Result));
	if (Result != EOnJoinSessionCompleteResult::Type::Success) return;

	AMenuPlayerController* TempController = GetOwningPlayer<AMenuPlayerController>();
	checkf(TempController, TEXT("FindedSessionWidget missed PlayerController!"));

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	checkf(SessionPtr.IsValid(), TEXT("FindedSessionWidget missed SessionPtr"));

	FString ConnectionInfo;
	SessionPtr.Get()->GetResolvedConnectString(SessionName, ConnectionInfo);
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, ConnectionInfo);

	APlayerController* TempPlayerController = GetWorld()->GetFirstPlayerController();
	checkf(TempPlayerController, TEXT("FindedSessionWidget missed PlayerController"))

	TempPlayerController->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
	//checkf(GetGameInstance()->ClientTravelToSession(TempController->NetPlayerIndex, SessionName), TEXT("CantTravel To session"));

	/*USessionUserWidget* TempWidget = CreateWidget<USessionUserWidget>(TempController, TempController->GetSessionWidgetClass());
	checkf(TempWidget, TEXT("FindedSessionWidget cant create session widget!"));

	TempController->SessionWidget = TempWidget;
	TempController->SessionWidget->GameLevelBlock->SetText(MapNameBlock->GetText());
	TempController->SessionWidget->SessionBlock->SetText(ServerNameBlock->GetText());
	TempController->SessionWidget->AddToPlayerScreen(1);*/
}
