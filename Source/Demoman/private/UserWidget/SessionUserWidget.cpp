// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget/SessionUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/PlayerUndecidedWidget.h"
#include "Game/CSNetworkSubsystem.h"
#include "Game/DemomanGameState.h"
#include "Player/GamePlayerController.h"
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

	tempState->OnUpdateWidgetDelegate.BindUObject(this, &USessionUserWidget::SetupPlayersInLobby);
	tempState->UpdateLobbyWidget();

	ConstructWidget();
}

void USessionUserWidget::OnJoinTeam(EPlayerLobbyTeam SelectedLobby)
{
	AGamePlayerState* tempPlayerState = Cast<AGamePlayerState>(
		GetOwningPlayer()->GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueId()));

	// Player press button before join 
	if (!tempPlayerState) return;

	tempPlayerState->SetPlayerLobbyState(SelectedLobby);
	SetupPlayersInLobby();

	if (SelectedLobby != Undecided) BJoinUndecided->SetVisibility(ESlateVisibility::Visible);
	else BJoinUndecided->SetVisibility(ESlateVisibility::Hidden);

	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->UpdateLobbyWidget();
}

void USessionUserWidget::OnStartGame_Implementation()
{
	// Check all player states
	for (APlayerState* tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedPlayerState = Cast<AGamePlayerState>(tempPlayer);
		checkf(CastedPlayerState, TEXT("SessionWidget cant get GamePlayerState"));
		if (CastedPlayerState->PlayerLobbyState == Undecided) return;
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
		FString TempString = CastedState->GetPlayerName() + " " + FString::FromInt(CastedState->PlayerLobbyState);
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

	for (APlayerState* tempPlayer : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedState = Cast<AGamePlayerState>(tempPlayer);

		switch (CastedState->GetPlayerLobbyState())
		{
		case Undecided:
		{
			UPlayerUndecidedWidget* tempWidget = CreateWidget<UPlayerUndecidedWidget>(GetOwningPlayer(), CastedState->PlayerUndecidedWidgetClass);
			tempWidget->PlayerName->SetText(FText::FromString(tempPlayer->GetPlayerName()));
			UndecidedScrollBox->AddChild(tempWidget);
		}
			break;
		case Red:
			BJoinRedColor->SetVisibility(ESlateVisibility::Collapsed);
			NameRedColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameRedColor->SetVisibility(ESlateVisibility::Visible);
			break;
		case Blue:
			BJoinBlueColor->SetVisibility(ESlateVisibility::Collapsed);
			NameBlueColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameBlueColor->SetVisibility(ESlateVisibility::Visible);
			break;
		case Green:
			BJoinGreenColor->SetVisibility(ESlateVisibility::Collapsed);
			NameGreenColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameGreenColor->SetVisibility(ESlateVisibility::Visible);
			break;
		case Yellow:
			BJoinYellowColor->SetVisibility(ESlateVisibility::Collapsed);
			NameYellowColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameYellowColor->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}

void USessionUserWidget::SetupDefaultSettings()
{
	UndecidedScrollBox->ClearChildren();

	NameYellowColor->SetVisibility(ESlateVisibility::Collapsed);
	NameGreenColor->SetVisibility(ESlateVisibility::Collapsed);
	NameBlueColor->SetVisibility(ESlateVisibility::Collapsed);
	NameRedColor->SetVisibility(ESlateVisibility::Collapsed);

	BJoinGreenColor->SetVisibility(ESlateVisibility::Visible);
	BJoinYellowColor->SetVisibility(ESlateVisibility::Visible);
	BJoinBlueColor->SetVisibility(ESlateVisibility::Visible);
	BJoinRedColor->SetVisibility(ESlateVisibility::Visible);
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
