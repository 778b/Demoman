// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"
#include "SessionUserWidget.generated.h"


class UTextBlock;
class UButton;

USTRUCT(BlueprintType)
struct FPlayerLobbyState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		APlayerController* Player = nullptr;

	UPROPERTY(BlueprintReadOnly)
		FColor PlayerColor = FColor::White;
};


UCLASS()
class DEMOMAN_API USessionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	void OnJoinSession(APlayerController* JoinedPlayer);
	void OnLeaveSession(APlayerController* LeavedPlayer);

	void OnJoinTeam(FColor SelectedColor, APlayerController* Player);
	void OnJoinUndecided(APlayerController* Leaver);

	void OnSelectNewMap();
	void OnStartGame();

	UFUNCTION(BlueprintCallable)
		void DrawDebugPlayers();

protected:
	virtual void OnRegisterPlayersCompleted(FName ServerName, const TArray< FUniqueNetIdRef >& Players, bool Success);

public:
	UPROPERTY(BlueprintReadOnly)
		TArray<APlayerController*> UndecidedPlayers;

	UPROPERTY(BlueprintReadOnly)
		TArray<FPlayerLobbyState> DecidedPlayers;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GameLevelBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* SessionBlock;

	FName PublicSessionName;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BJoinUndecided;

	//todo refactor this to massive of widgets
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

};
