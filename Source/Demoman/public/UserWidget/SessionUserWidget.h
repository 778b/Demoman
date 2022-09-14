// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"
#include "Player/GamePlayerState.h"
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

	UFUNCTION(BlueprintCallable)
		void OnJoinTeam(EPlayerLobbyTeam SelectedLobby);

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
		UButton* BJoinUndecided;

	// Cant refactor this to one massive
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BJoinRedColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BJoinBlueColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BJoinGreenColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BJoinYellowColor;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* NameRedColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* NameBlueColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* NameGreenColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* NameYellowColor;

	// We can use scrollbox, but anyway we got only 4 players
	// Refactoring this in next update, when will add map for 4+ players 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UScrollBox* UndecidedScrollBox;

};
