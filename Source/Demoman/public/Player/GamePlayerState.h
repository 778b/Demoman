// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DemomanUtils.h"
#include "GamePlayerState.generated.h"

class UPlayerUndecidedWidget;
class UPlayerDecidedWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatePlayerState);

DECLARE_MULTICAST_DELEGATE(FOnUpdateWidget);

UCLASS()
class DEMOMAN_API AGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGamePlayerState();

	UFUNCTION()
		EPlayerLobbyTeam GetPlayerLobbyState();

	UFUNCTION(Reliable, Server)
		void Server_UpdatePlayerWidget();
		void Server_UpdatePlayerWidget_Implementation();

	UFUNCTION(Reliable,	Client)
		void Client_UpdatePlayerWidget();
		void Client_UpdatePlayerWidget_Implementation();

	UFUNCTION(Reliable, Server)
		void SetPlayerLobbyState(EPlayerLobbyTeam newState);
		void SetPlayerLobbyState_Implementation(EPlayerLobbyTeam newState);

	UFUNCTION(Reliable, NetMulticast)
		void SetPlayerLobbyStateClient(EPlayerLobbyTeam newState);
		void SetPlayerLobbyStateClient_Implementation(EPlayerLobbyTeam newState);

	UFUNCTION(Reliable, Client)
		void OnPrepareGame();
		void OnPrepareGame_Implementation();

	UFUNCTION(Reliable, Client)
		void OnStartGame();
		void OnStartGame_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated)
		TEnumAsByte<EPlayerLobbyRole> PlayerLobbyRole;

	TSubclassOf<UPlayerUndecidedWidget> PlayerUndecidedWidgetClass;
	TSubclassOf<UPlayerDecidedWidget> PlayerDecidedWidgetClass;

	FOnUpdatePlayerState OnUpdatePlayerStateDelegate;
	
	FOnUpdateWidget OnUpdateWidgetDelegate;
protected:
	UPROPERTY(Replicated)
		TEnumAsByte<EPlayerLobbyTeam> PlayerLobbyState;
};
