// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/FindedSessionWidget.h"

#include "Components/TextBlock.h"
#include "Game/CSNetworkSubsystem.h"



void UFindedSessionWidget::TryJoinSession()
{
	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("FindedWidget missed NetworkSystem"));
	NetworkSys->JoinSession(SessionResult);
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


//checkf(GetGameInstance()->ClientTravelToSession(TempController->NetPlayerIndex, SessionName), TEXT("CantTravel To session"));

/*USessionUserWidget* TempWidget = CreateWidget<USessionUserWidget>(TempController, TempController->GetSessionWidgetClass());
checkf(TempWidget, TEXT("FindedSessionWidget cant create session widget!"));

TempController->SessionWidget = TempWidget;
TempController->SessionWidget->GameLevelBlock->SetText(MapNameBlock->GetText());
TempController->SessionWidget->SessionBlock->SetText(ServerNameBlock->GetText());
TempController->SessionWidget->AddToPlayerScreen(1);*/