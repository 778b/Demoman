// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DemomanUtils.h"
#include "PlayerDecidedWidget.generated.h"

DECLARE_DELEGATE_OneParam(OnChangeTeam, EPlayerLobbyTeam /*NewPlayerTeam*/)

class UImage;
class UButton;
class UTextBlock;
class AGamePlayerState;

UCLASS()
class DEMOMAN_API UPlayerDecidedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void OnJoinTeam(EPlayerLobbyTeam SelectedLobby);
	void SetupSettings(AGamePlayerState* Player, EPlayerLobbyRole OwnerRole);

	UFUNCTION(BlueprintCallable, Reliable, Server)
		void AddBotToGame();
		void AddBotToGame_Implementation();

	UFUNCTION(BlueprintCallable, Reliable, Server)
		void RemovePlayerFromGame();
		void RemovePlayerFromGame_Implementation();

	UFUNCTION()
		void OnClickedJoinButton();
	UFUNCTION()
		void OnClickedKickButton();
	UFUNCTION()
		void OnClickedAddBotButton();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* ColorSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* BJoinSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BAddBotToSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* BKickSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* NameSlot;

	OnChangeTeam FOnChangeTeamDelegate;

	TEnumAsByte<EPlayerLobbyTeam> PlayerLobbyColor;

	static EPlayerLobbyTeam CurrentColors;

private:

	const FSlateColor SlotColors[4]{	FLinearColor(1.f, 0.f, 0.f, 1.f),		// Red
										FLinearColor(0.f, 0.f, 1.f, 1.f),		// Blue
										FLinearColor(0.f, 0.7f, 0.f, 1.f), 		// Green
										FLinearColor(1.f, 0.8, 0.f, 1.f) };		// Yellow

};

EPlayerLobbyTeam UPlayerDecidedWidget::CurrentColors = Undecided;