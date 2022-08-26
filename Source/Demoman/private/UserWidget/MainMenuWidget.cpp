// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainMenuWidget.h"

#include "UObject/ConstructorHelpers.h"
#include "Player/GamePlayerController.h"
#include "UserWidget/FindedSessionWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UserWidget/SessionUserWidget.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& obj)
	: Super(obj)
{

}

void UMainMenuWidget::CreateGameSession(int32 NumPublicConnections, bool IsLAN, FString SessionName)
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid())
	{
		OnCompleteCreationSession.Execute(false);
		return;
	}
	FOnlineSessionSettings CurrentSessionSettings = FOnlineSessionSettings();
	
	CurrentSessionSettings.NumPublicConnections = NumPublicConnections;
	CurrentSessionSettings.bIsLANMatch = IsLAN;
	CurrentSessionSettings.NumPrivateConnections = 0;
	CurrentSessionSettings.bAllowJoinViaPresence = true;
	CurrentSessionSettings.bIsDedicated = false;
	CurrentSessionSettings.bUsesPresence = true;
	CurrentSessionSettings.bShouldAdvertise = true;

	CurrentSessionSettings.Set(SETTING_MAPNAME, FString("DemomanServerTextName"), EOnlineDataAdvertisementType::ViaOnlineService);

	SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionCreate));
	SessionPtr->CreateSession(GetOwningPlayer()->NetPlayerIndex, FName(SessionName), CurrentSessionSettings);

	CreateSessionButton->SetIsEnabled(false);
}

void UMainMenuWidget::FindGameSessions(int32 PlayerCount, int32 MaxSearchResult)
{
	UpdateSessionsButton->SetIsEnabled(false);

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid()) return;

	SessionPtr->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionFinding));

	//SessionPtr->AddOnCancelFindSessionsCompleteDelegate_Handle(FOnCancelFindSessionsCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionFinding));

	LastSearchSettings->MaxSearchResults = MaxSearchResult;
	LastSearchSettings->TimeoutInSeconds = 1000;

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (SessionPtr->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSearchSettings))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "Some Session was find");
	}
}

void UMainMenuWidget::OnCompleteSessionCreate(FName SessionName, bool result)
{
	CreateSessionButton->SetIsEnabled(true);
	if (!result) return;

	AGamePlayerController* TempController = GetOwningPlayer<AGamePlayerController>();
	checkf(TempController, TEXT("MainMenu was miss PlayerController!"));

	TempController->SessionWidget = CreateWidget<USessionUserWidget>(TempController, TempController->GetSessionWidgetClass());
	RemoveFromViewport();
	TempController->SessionWidget->AddToPlayerScreen(1);
}

void UMainMenuWidget::OnCompleteSessionFinding(bool bIsSuccess)
{
	check(bIsSuccess);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "SessionFinding is complete!");

	UpdateSessionsButton->SetIsEnabled(true);
	FindedSessionScrollBox->ClearChildren();

	AGamePlayerController* TempController = GetOwningPlayer<AGamePlayerController>();
	checkf(TempController, TEXT("MainMenu Missed Player Controller"));

	for (FOnlineSessionSearchResult& Session : LastSearchSettings.Get().SearchResults)
	{
		UFindedSessionWidget* TempWidget = CreateWidget<UFindedSessionWidget>(TempController, TempController->GetFindedSessionClass());
		checkf(TempWidget, TEXT("MainMenu Cant Create Widget of session"));

		TempWidget->SessionResult = Session;
		TempWidget->SetupWidgetSettings();

		FindedSessionScrollBox->AddChild(TempWidget);
	}

}
