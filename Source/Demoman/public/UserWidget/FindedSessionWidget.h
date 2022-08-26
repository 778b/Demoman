// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"

#include "FindedSessionWidget.generated.h"


class UTextBlock;

UCLASS()
class DEMOMAN_API UFindedSessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void TryJoinSession();

	void SetupWidgetSettings();

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);\

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* ServerName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* CurrentPlayers;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* Ping;

	FOnlineSessionSearchResult SessionResult;

	FOnJoinSessionCompleteDelegate JoinSessionDelegate;
};
