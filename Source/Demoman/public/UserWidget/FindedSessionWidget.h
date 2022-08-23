// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"

#include "FindedSessionWidget.generated.h"




UCLASS()
class DEMOMAN_API UFindedSessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void TryJoinSession();

public:
	int32 CurrentPlayers;
	// todo
	// servername
	// max players
	// ping
	FOnlineSessionSearchResult SessionResult;
};
