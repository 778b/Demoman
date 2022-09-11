// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerState.h"

#include "Game/DemomanGameState.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidget/PlayerUndecidedWidget.h"

AGamePlayerState::AGamePlayerState()
{
	ConstructorHelpers::FClassFinder<UPlayerUndecidedWidget> WidgetClass(TEXT("/Game/Widgets/WD_PlayerUndecided"));
	if (WidgetClass.Succeeded())
	{
		PlayerUndecidedWidgetClass = WidgetClass.Class;
	}
}

EPlayerLobbyTeam AGamePlayerState::GetPlayerLobbyState()
{
	return PlayerLobbyState;
}

void AGamePlayerState::SetPlayerLobbyState_Implementation(EPlayerLobbyTeam newState)
{
	PlayerLobbyState = newState;
	SetPlayerLobbyStateClient(newState);
}

void AGamePlayerState::SetPlayerLobbyStateClient_Implementation(EPlayerLobbyTeam newState)
{
	PlayerLobbyState = newState;
	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	tempState->UpdateLobbyWidget();
}

