// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuWidget.generated.h"


class UButton;
class UScrollBox;
class UFindedSessionWidget;

UCLASS()
class DEMOMAN_API UMainMenuWidget final : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& obj);

	UFUNCTION(BlueprintCallable)
		void CreateGameSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName);

	UFUNCTION(BlueprintCallable)
		void FindGameSessions(int32 MaxSearchResult, bool IsLAN);

	UFUNCTION(BlueprintImplementableEvent)
		void StartLoadingScreen();

	UFUNCTION(BlueprintImplementableEvent)
		void StopLoadingScreen();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* CreateSessionButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* UpdateSessionsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UScrollBox* FindedSessionScrollBox;

private:
	FName SelectedGameLevel;

	FOnlineSessionSearch* LastSearchSettings;

	virtual void OnCompleteSessionCreate(FName SessionName, bool Result);
	virtual void OnCompleteSessionFinding(bool bIsSuccess);
	virtual void OnCompleteJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
