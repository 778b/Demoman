// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerState.h"

#include "Game/DemomanGameState.h"
#include "Net/UnrealNetwork.h"
#include "Player/GamePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/GameInstance.h"
#include "UserWidget/PlayerUndecidedWidget.h"
#include "UserWidget/PlayerDecidedWidget.h"

AGamePlayerState::AGamePlayerState()
{
	ConstructorHelpers::FClassFinder<UPlayerUndecidedWidget> UndecidedWidgetClass(TEXT("/Game/Widgets/WD_PlayerUndecided"));
	if (UndecidedWidgetClass.Succeeded())
	{
		PlayerUndecidedWidgetClass = UndecidedWidgetClass.Class;
	}
	ConstructorHelpers::FClassFinder<UPlayerDecidedWidget> DecidedWidgetClass(TEXT("/Game/Widgets/WD_PlayerDecided"));
	if (DecidedWidgetClass.Succeeded())
	{
		PlayerDecidedWidgetClass = DecidedWidgetClass.Class;
	}

	ENetMode tempNetMode = GetNetMode();
	if (tempNetMode == ENetMode::NM_Standalone || tempNetMode == ENetMode::NM_ListenServer) PlayerLobbyRole = Admin;

}

EPlayerLobbyTeam AGamePlayerState::GetPlayerLobbyState()
{
	return PlayerLobbyState;
}

void AGamePlayerState::SetPlayerLobbyState_Implementation(EPlayerLobbyTeam newState)
{
	// Checking selected state for busy
	for (APlayerState* tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedState = Cast<AGamePlayerState>(tempPlayer);
		if (newState != Undecided && CastedState->PlayerLobbyState == newState)
		{
			ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
			tempState->UpdateLobbyWidget();
			return;
		};
	}
	// If check was success
	PlayerLobbyState = newState;
	SetPlayerLobbyStateClient(newState);
}

void AGamePlayerState::SetPlayerLobbyStateClient_Implementation(EPlayerLobbyTeam newState)
{
	PlayerLobbyState = newState;
	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	tempState->UpdateLobbyWidget();
}

void AGamePlayerState::OnPrepareGame_Implementation()
{
	
	AGamePlayerController* TempPlayerController = Cast<AGamePlayerController>(
		GetGameInstance()->GetFirstLocalPlayerController()->GetLocalPlayer()->GetPlayerController(GetWorld()));
	TempPlayerController->OnPrepareGame();
}

void AGamePlayerState::OnStartGame_Implementation()
{
	AGamePlayerController* TempPlayerController = Cast<AGamePlayerController>(
		GetGameInstance()->GetFirstLocalPlayerController()->GetLocalPlayer()->GetPlayerController(GetWorld()));
	TempPlayerController->OnStartGame();
}

void AGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayerState, PlayerLobbyState);
}

