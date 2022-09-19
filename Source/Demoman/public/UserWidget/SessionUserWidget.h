// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"
#include "DemomanUtils.h"
#include "SessionUserWidget.generated.h"


class UScrollBox;
class UTextBlock;
class UButton;
class UPlayerUndecidedWidget;


struct FPlayerLobbyState
{
	APlayerState* Player = nullptr;
	EPlayerLobbyTeam PlayerTeam = Undecided;
};


UCLASS()
class DEMOMAN_API USessionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Reliable, Server)
		void OnStartGame();
		void OnStartGame_Implementation();

	UFUNCTION(BlueprintCallable)
		void DrawDebugPlayers();
	UFUNCTION(BlueprintCallable)
		FName GetRoomName();

	// Blueprint event for native Construct
	UFUNCTION(BlueprintImplementableEvent)
		void ConstructWidget();

	// Blueprint Implementatation of function SetupPlayersInLobby()
	UFUNCTION(BlueprintCallable)
		void UpdateWidgetSettings();


protected:
	// Clearing all buttons and boxes 
	virtual void SetupDefaultSettings();
	// Setup Players settings in widget
	virtual void SetupPlayersInLobby();

	virtual void OnPostLoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer);
	virtual void OnLogoutEvent(AGameModeBase* GameMode, AController* Exiting);

public:
	TArray<FPlayerLobbyState> LobbyPlayers;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GameLevelBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* SessionBlock;

	FName PublicSessionName;
	

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BStartGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BJoinUndecided;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BAddDecidedSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UScrollBox* DecidedScrollBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UScrollBox* UndecidedScrollBox;
};
