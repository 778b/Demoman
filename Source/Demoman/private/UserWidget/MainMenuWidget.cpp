// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainMenuWidget.h"

#include "UObject/ConstructorHelpers.h"
#include "UserWidget/FindedSessionWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Interfaces/OnlineSessionInterface.h"




UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& obj)
	: Super(obj)
{
	ConstructorHelpers::FClassFinder<UUserWidget> gameLobbyWidgetClass(TEXT("/Game/Widgets/WD_GameLobby"));
	if (gameLobbyWidgetClass.Succeeded())
	{
		GameLobbyWidgetClass = gameLobbyWidgetClass.Class;
	}

	ConstructorHelpers::FClassFinder<UFindedSessionWidget> findedLobbyWidgetClass(TEXT("/Game/Widgets/WD_FindedSession"));
	if (findedLobbyWidgetClass.Succeeded())
	{
		FindedLobbyWidgetClass = findedLobbyWidgetClass.Class;
	}

	//CreateSessionButton = CreateDefaultSubobject<UButton>("B_CreateSession");
	//UpdateSessionsButton = CreateDefaultSubobject<UButton>("B_UpdateSessions");
	//FindedSessionScrollBox = CreateDefaultSubobject<UScrollBox>("SB_FindedSessions");
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

	SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionCreate));

	SessionPtr->CreateSession(GetOwningPlayer()->NetPlayerIndex, FName(SessionName), CurrentSessionSettings);

	CreateSessionButton->SetIsEnabled(false);
}

void UMainMenuWidget::FindGameSessions(int32 PlayerCount, int32 MaxSearchResult)
{
	UpdateSessionsButton->SetIsEnabled(false);

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid())
	{
		return;
	}
	SessionPtr->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionFinding));

	//SessionPtr->AddOnCancelFindSessionsCompleteDelegate_Handle(FOnCancelFindSessionsCompleteDelegate::CreateUObject(this, &UMainMenuWidget::OnCompleteSessionFinding));

	LastSearchSettings->MaxSearchResults = MaxSearchResult;
	LastSearchSettings->TimeoutInSeconds = 1000;

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (SessionPtr->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSearchSettings))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "Popa");
	}
}

void UMainMenuWidget::OnCompleteSessionCreate(FName SessionName, bool result)
{
	CreateSessionButton->SetIsEnabled(true);
	//if (!result) CreateSessionButton->SetIsEnabled(false);

	APlayerController* TempController = GetOwningPlayer();
	UUserWidget* TempWidget = CreateWidget<UUserWidget>(TempController, GameLobbyWidgetClass);
	RemoveFromViewport();
	TempWidget->AddToPlayerScreen(1);
}

void UMainMenuWidget::OnCompleteSessionFinding(bool bIsSuccess)
{
	if (!bIsSuccess)
	{
		check(false);
	}
	else GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "nice");

	UpdateSessionsButton->SetIsEnabled(true);

	APlayerController* TempController = GetOwningPlayer();
	checkf(TempController, TEXT("MainMenu Missed Player Controller"));

	//copy for VS debug;
	//todo delete;
	FOnlineSessionSettings debug;

	for (FOnlineSessionSearchResult& Session : LastSearchSettings.Get().SearchResults)
	{
		debug = Session.Session.SessionSettings;

		UFindedSessionWidget* TempWidget = CreateWidget<UFindedSessionWidget>(TempController, FindedLobbyWidgetClass);
		checkf(TempWidget, TEXT("MainMenu Cant Create Widget of session"));

		TempWidget->SessionResult = Session;

		FindedSessionScrollBox->AddChild(TempWidget);
	}

}
