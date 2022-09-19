// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainMenuWidget.h"

#include "UObject/ConstructorHelpers.h"
#include "Player/MenuPlayerController.h"
#include "UserWidget/FindedSessionWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CSNetworkSubsystem.h"
#include "Game/DemomanGameState.h"
#include "UserWidget/SessionUserWidget.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& obj)
	: Super(obj)
{
	if (!GetGameInstance()) return;
	UCSNetworkSubsystem* NetworkSystem = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSystem, TEXT("MainMenu missed NetworkSystem"));

	NetworkSystem->OnJoinSessionCompleteEvent.Add(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteJoinSession));
}

void UMainMenuWidget::CreateGameSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName)
{
	CreateSessionButton->SetIsEnabled(false);

	UCSNetworkSubsystem* NetworkSystem = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSystem, TEXT("MainMenu missed NetworkSystem"));

	SelectedGameLevel = LevelName;

	NetworkSystem->OnCreateSessionCompleteEvent.Add(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionCreate));
	NetworkSystem->CreateSession(NumPublicConnections, IsLAN, SessionName, LevelName);
}

void UMainMenuWidget::FindGameSessions(int32 MaxSearchResult, bool IsLAN)
{
	UpdateSessionsButton->SetIsEnabled(false);

	UCSNetworkSubsystem* NetworkSystem = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSystem, TEXT("MainMenu missed NetworkSystem"));

	NetworkSystem->OnFindSessionsCompleteEvent.AddUObject(this, &UMainMenuWidget::OnCompleteSessionFinding);
	NetworkSystem->FindSessions(MaxSearchResult, IsLAN);
}

void UMainMenuWidget::OnCompleteSessionCreate(FName SessionName, bool result)
{
	CreateSessionButton->SetIsEnabled(true);
	RemoveFromViewport();
}

void UMainMenuWidget::OnCompleteSessionFinding(bool bIsSuccess)
{
	if (bIsSuccess)
	{
		UpdateSessionsButton->SetIsEnabled(true);
		FindedSessionScrollBox->ClearChildren();

		UCSNetworkSubsystem* NetworkSystem = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
		checkf(NetworkSystem, TEXT("MainMenu missed NetworkSystem"));

		AMenuPlayerController* TempController = GetOwningPlayer<AMenuPlayerController>();
		checkf(TempController, TEXT("MainMenu missed Player Controller"));

		LastSearchSettings = NetworkSystem->GetLastSearchResult();
		for (FOnlineSessionSearchResult& Session : LastSearchSettings.SearchResults)
		{
			UFindedSessionWidget* TempWidget = CreateWidget<UFindedSessionWidget>(TempController, TempController->GetFindedSessionClass());
			checkf(TempWidget, TEXT("MainMenu cant create Widget of session"));

			TempWidget->SessionResult = Session;
			TempWidget->SetupWidgetSettings();

			FindedSessionScrollBox->AddChild(TempWidget);
		}
	}
}

void UMainMenuWidget::OnCompleteJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	RemoveFromViewport();
}
