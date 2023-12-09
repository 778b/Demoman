// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemomanGameState.h"

#include "Game/CSNetworkSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h" // IWYU pragma: keep
#include "OnlineSessionSettings.h"
#include "Player/GamePlayerController.h"
#include "DemomanUtils.h"
#include "TimerManager.h"
#include "Player/GamePlayerState.h"
#include "Player/PlayerCharacter.h"

DEFINE_LOG_CATEGORY(GameStateLog);

ADemomanGameState::ADemomanGameState()
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	if (!SessionPtr.IsValid())
	{
		return;
	}
	SessionPtr->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(
		this, &ADemomanGameState::OnRegisteredPlayersCompleted));
	SessionPtr->AddOnUnregisterPlayersCompleteDelegate_Handle(FOnUnregisterPlayersCompleteDelegate::CreateUObject(
		this, &ADemomanGameState::OnUnregisteredPlayersCompleted));

	FGameModeEvents::GameModePostLoginEvent.AddUObject(this, &ADemomanGameState::OnPostLoginEvent);
	FGameModeEvents::GameModeLogoutEvent.AddUObject(this, &ADemomanGameState::OnLogoutEvent);


}

void ADemomanGameState::UpdateWidget_Implementation()
{
	APlayerState* tempPlayerState = nullptr;

	if (GetWorld() && GetGameInstance())
	{
		// Getting local player state
		tempPlayerState = GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueIdRepl());
	}

	UE_LOG(GameStateLog, Display, TEXT("[%s] Widget update"), 
		tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()): TEXT("UnknownMode"));

	OnUpdateWidgetDelegate.ExecuteIfBound();
}

void ADemomanGameState::Server_StartGameTimer_Implementation()
{
	for (APlayerState* tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AController* tempController = tempPlayer->GetOwner<AController>();
		checkf(tempController, TEXT("GameState cant get Controller"));

		AActor* tempActor = GetWorld()->GetAuthGameMode()->ChoosePlayerStart(tempController);
		checkf(tempActor, TEXT("GameState cant find PlayerStart"));

		APlayerCharacter* tempPlayerPawn = GetWorld()->SpawnActor<APlayerCharacter>(
			tempActor->GetActorLocation(), FRotator(0.f));
		checkf(tempPlayerPawn, TEXT("GameState cant create PlayerPawn"));

		tempActor->Destroy(true);
		tempController->Possess(tempPlayerPawn);
		tempPlayerPawn->SetAutoTeamMaterial();

		AGamePlayerState* TempPlayerState = Cast<AGamePlayerState>(tempPlayer);
		checkf(TempPlayerState, TEXT("GameState cant find player state"));
		TempPlayerState->OnPrepareGame();
	}


	GetWorldTimerManager().SetTimer(StartTimer, FTimerDelegate::CreateUObject(this, &ADemomanGameState::StartGame),	5.f, false, -1.f);
}

void ADemomanGameState::StartGame()
{
	APlayerState* tempPlayerState = GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
		GetGameInstance()->GetPrimaryPlayerUniqueIdRepl());

	UE_LOG(GameStateLog, Display, TEXT("[%s] Game start"), tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()) : TEXT("UnknownMode"));
	for (APlayerState* tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* TempPlayerState = Cast<AGamePlayerState>(tempPlayer);
		checkf(TempPlayerState, TEXT("GameState cant find player state"));

		TempPlayerState->OnStartGame();
	}
}

void ADemomanGameState::OnPostLoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	if (!GetWorld()) return;

	APlayerState* tempPlayerState = GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
		GetGameInstance()->GetPrimaryPlayerUniqueIdRepl());

	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	FOnlineSessionSettings* TempSessionSettings = SessionPtr->GetSessionSettings(NetworkSys->LastSessionName);
	++TempSessionSettings->NumPrivateConnections;
	
	UE_LOG(GameStateLog, Display, TEXT("[%s] Loggin event, connected %s"),
		tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()) : TEXT("UnknownMode"), *NewPlayer->GetHumanReadableName());

	SessionPtr->UpdateSession(NetworkSys->LastSessionName, *TempSessionSettings);

	Server_UpdateWidget();
}

void ADemomanGameState::OnLogoutEvent(AGameModeBase* GameMode, AController* Exiting)
{
	if (!GetWorld()) return;

	APlayerState* tempPlayerState = GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
		GetGameInstance()->GetPrimaryPlayerUniqueIdRepl());

	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	FOnlineSessionSettings* TempSessionSettings = SessionPtr->GetSessionSettings(NetworkSys->LastSessionName);
	--TempSessionSettings->NumPrivateConnections;

	UE_LOG(GameStateLog, Display, TEXT("[%s] Logout event, disconnected %s"),
		tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()) : TEXT("UnknownMode"), *Exiting->GetHumanReadableName());

	SessionPtr->UpdateSession(NetworkSys->LastSessionName, *TempSessionSettings);

	Server_UpdateWidget();
}

void ADemomanGameState::Server_UpdateWidget_Implementation()
{

	UE_LOG(GameStateLog, Display, TEXT("[%s] Server_UpdateWidget"));
	UpdateWidget();
}

void ADemomanGameState::OnRegisteredPlayersCompleted(FName sessionName, const TArray<FUniqueNetIdRef>& Players, bool Result)
{
	APlayerState* tempPlayerState = GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
		GetGameInstance()->GetPrimaryPlayerUniqueIdRepl());

	UE_LOG(GameStateLog, Display, TEXT("[%s] Registered complete"),
		tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()) : TEXT("UnknownMode"));
	for (auto player : Players)
	{
		UE_LOG(GameStateLog, Display, TEXT("[%s] Registered event, player %s")
			, tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()) : TEXT("UnknownMode"), *player.Get().ToDebugString());
	}
	Server_UpdateWidget();
}

void ADemomanGameState::OnUnregisteredPlayersCompleted(FName sessionName, const TArray<FUniqueNetIdRef>& Players, bool Result)
{
	APlayerState* tempPlayerState = GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
		GetGameInstance()->GetPrimaryPlayerUniqueIdRepl());
	UE_LOG(GameStateLog, Display, TEXT("[%s] Unregistered complete"), 
		tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()) : TEXT("UnknownMode"));
	for (auto player : Players)
	{
		UE_LOG(GameStateLog, Display, TEXT("[%s] Unregistered event, player %s"), 
			tempPlayerState ? *FDemomanUtils::ConvertNetModeToString(tempPlayerState->GetNetMode()) : TEXT("UnknownMode"), *player.Get().ToDebugString());
	}
	Server_UpdateWidget();
}