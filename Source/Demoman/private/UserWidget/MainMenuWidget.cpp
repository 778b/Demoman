// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainMenuWidget.h"

#include "UObject/ConstructorHelpers.h"
#include "Player/MenuPlayerController.h"
#include "UserWidget/FindedSessionWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/SessionUserWidget.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& obj)
	: Super(obj)
{

}

void UMainMenuWidget::CreateGameSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName)
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid())
	{
		OnCompleteCreationSession.Execute(false);
		return;
	}
	SelectedGameLevel = LevelName;

	FOnlineSessionSettings CurrentSessionSettings = FOnlineSessionSettings();
	
	CurrentSessionSettings.NumPublicConnections = NumPublicConnections;
	CurrentSessionSettings.bIsLANMatch = IsLAN;
	CurrentSessionSettings.NumPrivateConnections = 0;
	CurrentSessionSettings.bAllowJoinViaPresence = true;
	CurrentSessionSettings.bIsDedicated = false;
	CurrentSessionSettings.bUsesPresence = true;
	CurrentSessionSettings.bShouldAdvertise = true;

	// Using FString constructor for avoiding implicit transform our string to FOnlineSessionSetting
	CurrentSessionSettings.Set(SETTING_GAMEMODE, FString(SessionName), EOnlineDataAdvertisementType::Type::ViaOnlineService);
	CurrentSessionSettings.Set(SETTING_MAPNAME, LevelName.ToString(),  EOnlineDataAdvertisementType::Type::ViaOnlineService);

	SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(
			this, &UMainMenuWidget::OnCompleteSessionCreate));
	SessionPtr->CreateSession(GetOwningPlayer()->NetPlayerIndex, FName(SessionName), CurrentSessionSettings);

	CreateSessionButton->SetIsEnabled(false);
}

void UMainMenuWidget::FindGameSessions(int32 PlayerCount, int32 MaxSearchResult)
{
	UpdateSessionsButton->SetIsEnabled(false);

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid()) return;

	SessionPtr->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(
			this, &UMainMenuWidget::OnCompleteSessionFinding));
	//SessionPtr->AddOnCancelFindSessionsCompleteDelegate_Handle(FOnCancelFindSessionsCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionFinding));

	LastSearchSettings->MaxSearchResults = MaxSearchResult;
	LastSearchSettings->TimeoutInSeconds = 1000;

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionPtr->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSearchSettings);
}

void UMainMenuWidget::OnCompleteSessionCreate(FName SessionName, bool result)
{
	CreateSessionButton->SetIsEnabled(true);
	if (!result) return;

	AMenuPlayerController* TempController = GetOwningPlayer<AMenuPlayerController>();
	checkf(TempController, TEXT("MainMenu missed PlayerController!"));

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	check(SessionPtr.IsValid());

	FString MapName;
	check(SessionPtr->GetSessionSettings(SessionName)->Get(SETTING_MAPNAME, MapName));

	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName));
	//TempController->SessionWidget = CreateWidget<USessionUserWidget>(TempController, TempController->GetSessionWidgetClass());
	//checkf(TempController->SessionWidget, TEXT("MainMenu cant create SessionWidget!"));

	//RemoveFromViewport();
	//TempController->SessionWidget->GameLevelBlock->SetText(FText::FromString(SelectedGameLevel.ToString()));
	//TempController->SessionWidget->PublicSessionName = SessionName;
	//TempController->SessionWidget->AddToPlayerScreen(1);
}

void UMainMenuWidget::OnCompleteSessionFinding(bool bIsSuccess)
{
	check(bIsSuccess);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "SessionFinding is complete!");

	UpdateSessionsButton->SetIsEnabled(true);
	FindedSessionScrollBox->ClearChildren();

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	checkf(SessionPtr.IsValid(), TEXT("MainMenu cant find SessionPtr!"));
	SessionPtr->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateLambda(
		[this](FName SessionName, EOnJoinSessionCompleteResult::Type Result)
		{
			RemoveFromViewport();
		}
	));

	AMenuPlayerController* TempController = GetOwningPlayer<AMenuPlayerController>();
	checkf(TempController, TEXT("MainMenu Missed Player Controller"));

	FOnlineSessionSearchResult debug;

	for (FOnlineSessionSearchResult& Session : LastSearchSettings.Get().SearchResults)
	{
		debug = Session;

		UFindedSessionWidget* TempWidget = CreateWidget<UFindedSessionWidget>(TempController, TempController->GetFindedSessionClass());
		checkf(TempWidget, TEXT("MainMenu Cant Create Widget of session"));

		TempWidget->SessionResult = Session;
		TempWidget->SetupWidgetSettings();

		FindedSessionScrollBox->AddChild(TempWidget);
	}

}
