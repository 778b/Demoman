// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemomanGameState.h"

#include "Game/CSNetworkSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Player/GamePlayerController.h"
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
#ifdef UE_BUILD_DEVELOPMENT
	if (GetWorld() && GetGameInstance())
	{
		// Getting local player state
		APlayerState* tempPlayerState = GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueId());

		FString TempName = tempPlayerState ? tempPlayerState->GetPlayerName() : TEXT("UnknownPlayerName");

		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta, TempName);

	}
#endif

	UE_LOG(GameStateLog, Display, TEXT("Widget update"));

	OnUpdateWidgetDelegate.ExecuteIfBound();
}


void ADemomanGameState::UpdateLobbyWidget_Implementation()
{
	UE_LOG(GameStateLog, Display, TEXT("Lobby widget update"));
	OnUpdateWidgetDelegate.ExecuteIfBound();
	UpdateWidget();
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
	for (APlayerState* tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* TempPlayerState = Cast<AGamePlayerState>(tempPlayer);
		checkf(TempPlayerState, TEXT("GameState cant find player state"));

		UE_LOG(GameStateLog, Display, TEXT("Game start"));
		TempPlayerState->OnStartGame();
	}
}

void ADemomanGameState::OnPostLoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	if (!GetWorld()) return;

	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	FOnlineSessionSettings* TempSessionSettings = SessionPtr->GetSessionSettings(NetworkSys->LastSessionName);
	++TempSessionSettings->NumPrivateConnections;
	
	UE_LOG(GameStateLog, Display, TEXT("Loggin event, connected %s"), *NewPlayer->GetHumanReadableName());

	SessionPtr->UpdateSession(NetworkSys->LastSessionName, *TempSessionSettings);
}

void ADemomanGameState::OnLogoutEvent(AGameModeBase* GameMode, AController* Exiting)
{
	if (!GetWorld()) return;

	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));

	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	FOnlineSessionSettings* TempSessionSettings = SessionPtr->GetSessionSettings(NetworkSys->LastSessionName);
	--TempSessionSettings->NumPrivateConnections;

	UE_LOG(GameStateLog, Display, TEXT("Logout event, disconnected %s"), *Exiting->GetHumanReadableName());

	SessionPtr->UpdateSession(NetworkSys->LastSessionName, *TempSessionSettings);
}

void ADemomanGameState::OnRegisteredPlayersCompleted(FName sessionName, const TArray<FUniqueNetIdRef>& Players, bool Result)
{
	UE_LOG(GameStateLog, Display, TEXT("Registered complete"));
	for (auto player : Players)
	{
		UE_LOG(GameStateLog, Display, TEXT("Registered event, player %s"), *player.Get().ToDebugString());
	}
	UpdateWidget();
}

void ADemomanGameState::OnUnregisteredPlayersCompleted(FName sessionName, const TArray<FUniqueNetIdRef>& Players, bool Result)
{
	UE_LOG(GameStateLog, Display, TEXT("Unregistered complete"));
	for (auto player : Players)
	{
		UE_LOG(GameStateLog, Display, TEXT("Unregistered event, player %s"), *player.Get().ToDebugString());
	}
	UpdateWidget();
}