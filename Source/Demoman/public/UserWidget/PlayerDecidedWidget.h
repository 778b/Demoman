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
	virtual bool Initialize() override;

	void OnJoinTeam(EPlayerLobbyTeam SelectedLobby);
	void SetupSettings(AGamePlayerState* Player, EPlayerLobbyRole OwnerRole);
	const void UpdateColor(EPlayerLobbyTeam NewTeam);

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

	static const FSlateColor SlotColors[8];

	static EPlayerLobbyTeam CurrentColors;

private:

	

};