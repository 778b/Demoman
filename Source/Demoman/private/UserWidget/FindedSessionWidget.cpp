// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/FindedSessionWidget.h"

#include "Components/TextBlock.h"
#include "Player/GamePlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UserWidget/SessionUserWidget.h"


void UFindedSessionWidget::TryJoinSession()
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid())
	{
		return;
	}
	//SessionPtr->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UFindedSessionWidget::OnJoinSessionCompleted));
	TSharedRef<UUserWidget> tempShared = MakeShareable(this);

	JoinSessionDelegate.BindUObject(this, &UFindedSessionWidget::OnJoinSessionCompleted);

	SessionPtr->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegate);
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	SessionPtr->JoinSession(*localPlayer->GetPreferredUniqueNetId(), FName(), SessionResult);

}


void UFindedSessionWidget::SetupWidgetSettings()
{
	FString RoomName;
	checkf(SessionResult.Session.SessionSettings.Get(SETTING_MAPNAME, RoomName), TEXT("FindedSession had wrong session FName!"));

	FText Players = FText::FromString(FString::FromInt(SessionResult.Session.NumOpenPrivateConnections) + "/"	
										+ FString::FromInt(SessionResult.Session.NumOpenPublicConnections));

	Ping->SetText(FText::AsNumber(SessionResult.PingInMs));
	CurrentPlayers->SetText(Players);
	ServerName->SetText(FText::FromString(RoomName));

}

void UFindedSessionWidget::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "Join session is complete!");
	if (Result != EOnJoinSessionCompleteResult::Type::Success) return;

	AGamePlayerController* TempController = GetOwningPlayer<AGamePlayerController>();
	checkf(TempController, TEXT("FindedSessionWidget missed PlayerController!"));
	
	TempController->SessionWidget = CreateWidget<USessionUserWidget>(TempController, TempController->GetSessionWidgetClass());
	TempController->SessionWidget->AddToPlayerScreen(1);
}
