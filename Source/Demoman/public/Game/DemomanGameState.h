// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "GameFramework/GameStateBase.h"
#include "DemomanGameState.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(GameStateLog, Log, All);

UCLASS()
class DEMOMAN_API ADemomanGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADemomanGameState();

	UFUNCTION(Reliable, Server)
		void Server_UpdateWidget();
		void Server_UpdateWidget_Implementation();

	UFUNCTION(Reliable, Server)
		void Server_StartGameTimer();
		void Server_StartGameTimer_Implementation();

protected:
	virtual void OnRegisteredPlayersCompleted(FName sessionName, const TArray< FUniqueNetIdRef >& Players, bool Result);
	virtual void OnUnregisteredPlayersCompleted(FName sessionName, const TArray< FUniqueNetIdRef >& Players, bool Result);

	virtual void StartGame();

	virtual void OnPostLoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer);
	virtual void OnLogoutEvent(AGameModeBase* GameMode, AController* Exiting);

private:
	UFUNCTION(Reliable, NetMulticast)
		void UpdateWidget();
		void UpdateWidget_Implementation();



private:
	FTimerHandle StartTimer;
};
