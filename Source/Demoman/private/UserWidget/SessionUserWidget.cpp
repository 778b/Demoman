// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget/SessionUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/PlayerUndecidedWidget.h"
#include "UserWidget/PlayerDecidedWidget.h"
#include "Game/CSNetworkSubsystem.h"
#include "Game/DemomanGameState.h"
#include "Player/GamePlayerController.h"
#include "Player/GamePlayerState.h"
#include "Player/PlayerCharacter.h"

void USessionUserWidget::NativeConstruct()
{
	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));
	PublicSessionName = NetworkSys->LastSessionName;

	FGameModeEvents::GameModePostLoginEvent.AddUObject(this, &USessionUserWidget::OnPostLoginEvent);
	FGameModeEvents::GameModeLogoutEvent.AddUObject(this, &USessionUserWidget::OnLogoutEvent);

	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));

	if (const AGamePlayerState* tempPlayerState = GetOwningPlayerState<AGamePlayerState>())
	{
		if (tempPlayerState->PlayerLobbyRole == Admin)
		{
			BStartGame->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			BStartGame->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	BJoinUndecided->OnClicked.AddDynamic(this, &USessionUserWidget::OnClickedUndecidedButton);

	tempState->OnUpdateWidgetDelegate.BindUObject(this, &USessionUserWidget::SetupPlayersInLobby);
	tempState->UpdateLobbyWidget();

	ConstructWidget();
}

void USessionUserWidget::OnStartGame_Implementation()
{
	// Check all player states
	for (APlayerState* tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedPlayerState = Cast<AGamePlayerState>(tempPlayer);
		checkf(CastedPlayerState, TEXT("SessionWidget cant get GamePlayerState"));
		if (CastedPlayerState->GetPlayerLobbyState() == Undecided) return;
	}
	
	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->Server_StartGameTimer();

	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));
	NetworkSys->StartSession(PublicSessionName);
}


void USessionUserWidget::DrawDebugPlayers()
{
#ifdef UE_BUILD_DEVELOPMENT
	for (APlayerState* tempPlayer : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedState = Cast<AGamePlayerState>(tempPlayer);
		FString TempString = CastedState->GetPlayerName() + " " + FString::FromInt(CastedState->GetPlayerLobbyState());
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TempString);
	}
#endif
}

FName USessionUserWidget::GetRoomName()
{
	return PublicSessionName;
}

void USessionUserWidget::SetupPlayersInLobby()
{
	SetupDefaultSettings();

	// Getting TRUE local player for our client
	AGamePlayerState* OwnerPlayerState = Cast<AGamePlayerState>(
		GetOwningPlayer()->GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueId()));
	checkf(OwnerPlayerState, TEXT("SessionWidget missed OwnerPlayerState"));
	if (!OwnerPlayerState) return;

	int32 DecidedPlayersCount = 0;
	// Nulling static var of colors of decided widgets
	UPlayerDecidedWidget::CurrentColors = Undecided;

	// Creating widget to Decided and Undedcided players
	for (APlayerState* tempPlayer : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedState = Cast<AGamePlayerState>(tempPlayer);

		if (CastedState->GetPlayerLobbyState() == Undecided)
		{
			// Local namespace for allowing create widget
			UPlayerUndecidedWidget* tempWidget = CreateWidget<UPlayerUndecidedWidget>(GetOwningPlayer(), CastedState->PlayerUndecidedWidgetClass);
			tempWidget->PlayerName->SetText(FText::FromString(tempPlayer->GetPlayerName()));
			UndecidedScrollBox->AddChild(tempWidget);
		}
		else
		{
			UPlayerDecidedWidget* tempWidget = CreateWidget<UPlayerDecidedWidget>(GetOwningPlayer(), CastedState->PlayerDecidedWidgetClass);
			tempWidget->FOnChangeTeamDelegate.BindUObject(this, &USessionUserWidget::OnOwnerChangedTeam);
			tempWidget->SetupSettings(CastedState, OwnerPlayerState->PlayerLobbyRole);
			DecidedScrollBox->AddChild(tempWidget);
			++DecidedPlayersCount;
		}
	}

	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	checkf(SessionPtr.IsValid(), TEXT("SessionWidget missed SEssionPtr"));

	FOnlineSessionSettings* TempSessionSettings = SessionPtr->GetSessionSettings(NetworkSys->LastSessionName);
	checkf(TempSessionSettings, TEXT("SessionWidget missed SessionSettings"));


	// Creating empty decided slot widget
	for (int32 i = DecidedPlayersCount; i < TempSessionSettings->NumPublicConnections; ++i)
	{
		UPlayerDecidedWidget* tempWidget = CreateWidget<UPlayerDecidedWidget>(GetOwningPlayer(), OwnerPlayerState->PlayerDecidedWidgetClass);
		checkf(tempWidget, TEXT("SessionWidget missed slots"));
		tempWidget->FOnChangeTeamDelegate.BindUObject(this, &USessionUserWidget::OnOwnerChangedTeam);
		tempWidget->SetupSettings(nullptr, OwnerPlayerState->PlayerLobbyRole);
		DecidedScrollBox->AddChild(tempWidget);
	}
}

void USessionUserWidget::SetupDefaultSettings()
{
	UndecidedScrollBox->ClearChildren();

	DecidedScrollBox->ClearChildren();
	
}

void USessionUserWidget::UpdateWidgetSettings()
{
	SetupPlayersInLobby();
}

void USessionUserWidget::OnPostLoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	SetupPlayersInLobby();
}

void USessionUserWidget::OnLogoutEvent(AGameModeBase* GameMode, AController* Exiting)
{
	SetupPlayersInLobby();
}

void USessionUserWidget::OnOwnerChangedTeam(EPlayerLobbyTeam NewPlayerTeam)
{
	if (NewPlayerTeam != Undecided)
	{
		BJoinUndecided->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BJoinUndecided->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USessionUserWidget::OnClickedUndecidedButton()
{
	AGamePlayerState* tempPlayerState = Cast<AGamePlayerState>(
		GetOwningPlayer()->GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueId()));

	tempPlayerState->SetPlayerLobbyState(Undecided);
	OnOwnerChangedTeam(Undecided);

	// Local update widget
	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->UpdateLobbyWidget();
}
