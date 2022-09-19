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
				// Local namespace for allowing create widget
				UPlayerUndecidedWidget* tempWidget = CreateWidget<UPlayerUndecidedWidget>(GetOwningPlayer(), CastedState->PlayerUndecidedWidgetClass);
				tempWidget->PlayerName->SetText(FText::FromString(tempPlayer->GetPlayerName()));
				UndecidedScrollBox->AddChild(tempWidget);
				break;
			}
		default:
			{
				UPlayerDecidedWidget* tempWidget = CreateWidget<UPlayerDecidedWidget>(GetOwningPlayer(), CastedState->PlayerDecidedWidgetClass);
				tempWidget->SetupSettings(CastedState);
				DecidedScrollBox->AddChild(tempWidget);
				break;
			}
		}
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
