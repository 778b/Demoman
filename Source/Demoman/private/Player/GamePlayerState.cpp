// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerState.h"

#include "Game/DemomanGameState.h"
#include "Net/UnrealNetwork.h"
#include "Player/GamePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/GameInstance.h"
#include "TimerManager.h"
#include "UserWidget/PlayerUndecidedWidget.h"
#include "UserWidget/PlayerDecidedWidget.h"

DEFINE_LOG_CATEGORY(PlayerStateLog);

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

void AGamePlayerState::Server_UpdatePlayerWidget_Implementation()
{
	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->Server_UpdateWidget();
}

void AGamePlayerState::Client_UpdatePlayerWidget_Implementation()
{
	OnUpdateWidgetDelegate.Broadcast();
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
			checkf(tempState, TEXT("SessionWidget missed GameState"));
			tempState->Server_UpdateWidget();
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
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->Server_UpdateWidget();
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

void AGamePlayerState::BeginPlay()
{
	GetWorld()->GetTimerManager().SetTimer(WaitAfterJoin, FTimerDelegate::CreateUObject(this, &AGamePlayerState::Server_UpdatePlayerWidget), 5.f, false);
	//ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	//checkf(tempState, TEXT("SessionWidget missed GameState"));
	//tempState->Server_UpdateWidget();
}

void AGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayerState, PlayerLobbyState);
	DOREPLIFETIME(AGamePlayerState, PlayerLobbyRole);
}

