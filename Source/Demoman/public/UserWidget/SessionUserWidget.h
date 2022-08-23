// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"
#include "SessionUserWidget.generated.h"


USTRUCT(BlueprintType)
struct FPlayerLobbyState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		APlayerController* Player;

	UPROPERTY(BlueprintReadOnly)
		FColor PlayerColor;
};


UCLASS()
class DEMOMAN_API USessionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnJoinSession(APlayerController* JoinedPlayer);
	void OnLeaveSession(APlayerController* LeavedPlayer);

	void OnJoinTeam(FColor SelectedColor, APlayerController* Player);
	void OnJoinUndecided(APlayerController* Leaver);

	void OnSelectNewMap();
	void OnStartGame();

public:
	UPROPERTY(BlueprintReadOnly)
		TArray<APlayerController*> UndecidedPlayers;

	UPROPERTY(BlueprintReadOnly)
		TArray<FPlayerLobbyState> DecidedPlayers;

};
