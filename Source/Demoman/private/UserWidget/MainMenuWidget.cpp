// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainMenuWidget.h"

#include "UObject/ConstructorHelpers.h"
#include "Player/MenuPlayerController.h"
#include "UserWidget/FindedSessionWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Game/CSNetworkSubsystem.h"
#include "UserWidget/SessionUserWidget.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& obj)
	: Super(obj)
{

}

void UMainMenuWidget::CreateGameSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName)
{
	CreateSessionButton->SetIsEnabled(false);

	UCSNetworkSubsystem* NetworkSystem = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSystem, TEXT("MainMenu missed NetworkSystem")); // crashed

	SelectedGameLevel = LevelName;

	NetworkSystem->OnCreateSessionCompleteEvent.Add(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionCreate));
	NetworkSystem->CreateSession(NumPublicConnections, IsLAN, SessionName, LevelName);
}

void UMainMenuWidget::FindGameSessions(int32 PlayerCount, int32 MaxSearchResult)
{
	UpdateSessionsButton->SetIsEnabled(false);

	UCSNetworkSubsystem* NetworkSystem = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSystem, TEXT("MainMenu missed NetworkSystem")); // crashed

	NetworkSystem->OnFindSessionsCompleteEvent.AddUObject(this, &UMainMenuWidget::OnCompleteSessionFinding);
	NetworkSystem->FindSessions(PlayerCount, MaxSearchResult);
}

void UMainMenuWidget::OnCompleteSessionCreate(FName SessionName, bool result)
{
	CreateSessionButton->SetIsEnabled(true);

	//TempController->SessionWidget = CreateWidget<USessionUserWidget>(TempController, TempController->GetSessionWidgetClass());
	//checkf(TempController->SessionWidget, TEXT("MainMenu cant create SessionWidget!"));

	//RemoveFromViewport();
	//TempController->SessionWidget->GameLevelBlock->SetText(FText::FromString(SelectedGameLevel.ToString()));
	//TempController->SessionWidget->PublicSessionName = SessionName;
	//TempController->SessionWidget->AddToPlayerScreen(1);
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
