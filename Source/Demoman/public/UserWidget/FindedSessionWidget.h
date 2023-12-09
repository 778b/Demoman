// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "FindedSessionWidget.generated.h"


class UTextBlock;

UCLASS()
class DEMOMAN_API UFindedSessionWidget final : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void TryJoinSession();

	void SetupWidgetSettings();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* ServerNameBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* MapNameBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* CurrentPlayersBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* PingBlock;

	FName PublicSessionName;
	FOnlineSessionSearchResult SessionResult;

private:
	FString RoomName;
};
