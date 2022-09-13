// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GamePlayerState.generated.h"

class UPlayerUndecidedWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatePlayerState);

UENUM()
enum EPlayerLobbyTeam
{
	Undecided,
	Red,
	Blue,
	Green,
	Yellow
};

UCLASS()
class DEMOMAN_API AGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGamePlayerState();

	UFUNCTION()
		EPlayerLobbyTeam GetPlayerLobbyState();

	UFUNCTION(Reliable, Server)
		void SetPlayerLobbyState(EPlayerLobbyTeam newState);
		void SetPlayerLobbyState_Implementation(EPlayerLobbyTeam newState);

	UFUNCTION(Reliable, NetMulticast)
		void SetPlayerLobbyStateClient(EPlayerLobbyTeam newState);
		void SetPlayerLobbyStateClient_Implementation(EPlayerLobbyTeam newState);


public:
	UPROPERTY()
	TEnumAsByte<EPlayerLobbyTeam> PlayerLobbyState;

	TSubclassOf<UPlayerUndecidedWidget> PlayerUndecidedWidgetClass;

	FOnUpdatePlayerState OnUpdatePlayerStateDelegate;
};
