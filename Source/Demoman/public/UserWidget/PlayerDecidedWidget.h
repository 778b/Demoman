// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DemomanUtils.h"
#include "PlayerDecidedWidget.generated.h"

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
	void SetupSettings(AGamePlayerState* Player);

	UFUNCTION(BlueprintCallable, Reliable, Server)
		void AddBotToGame();
		void AddBotToGame_Implementation();

	UFUNCTION(BlueprintCallable, Reliable, Server)
		void RemovePlayerFromGame();
		void RemovePlayerFromGame_Implementation();

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

	TEnumAsByte<EPlayerLobbyTeam> PlayerLobbyColor;

private:
	const FSlateColor BlueColor	= FLinearColor(0.f, 0.f, 1.f, 1.f);
	const FSlateColor RedColor	= FLinearColor(1.f, 0.f, 0.f, 1.f);
	const FSlateColor YellowColor = FLinearColor(1.f, 0.8, 0.f, 1.f);
	const FSlateColor GreenColor	= FLinearColor(0.f, 0.7f, 0.f, 1.f);
};
