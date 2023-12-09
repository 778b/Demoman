// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/FindedSessionWidget.h"

#include "Components/TextBlock.h"
#include "Game/CSNetworkSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystem.h" // IWYU pragma: keep



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

	FText Players = FText::FromString(FString::FromInt(SessionResult.Session.SessionSettings.NumPrivateConnections) + "/"	
										+ FString::FromInt(SessionResult.Session.SessionSettings.NumPublicConnections));

	PingBlock->SetText(FText::AsNumber(SessionResult.PingInMs));
	CurrentPlayersBlock->SetText(Players);
	ServerNameBlock->SetText(FText::FromString(RoomName));
	MapNameBlock->SetText(FText::FromString(MapName));
}
