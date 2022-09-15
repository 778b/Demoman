// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DemomanGameState.generated.h"


DECLARE_DELEGATE(FOnUpdateWidget);


UCLASS()
class DEMOMAN_API ADemomanGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADemomanGameState();

	UFUNCTION(Reliable, Server)
		void UpdateLobbyWidget();
		void UpdateLobbyWidget_Implementation();

	UFUNCTION(Reliable, Server)
		void Server_StartGameTimer();
		void Server_StartGameTimer_Implementation();

protected:
	virtual void OnRegisteredPlayersCompleted(FName sessionName, const TArray< FUniqueNetIdRef >& Players, bool Result);
	virtual void OnUnregisteredPlayersCompleted(FName sessionName, const TArray< FUniqueNetIdRef >& Players, bool Result);

private:
	UFUNCTION(Reliable, NetMulticast)
		void UpdateWidget();
		void UpdateWidget_Implementation();

public:
	FOnUpdateWidget OnUpdateWidgetDelegate;

private:
	FTimerHandle StartTimer;
};
