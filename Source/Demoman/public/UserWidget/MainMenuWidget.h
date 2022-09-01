// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemUtils.h"
#include "MainMenuWidget.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCompleteCreationSession, bool, bIsSuccess);


class UButton;
class UScrollBox;
class UFindedSessionWidget;

UCLASS()
class DEMOMAN_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& obj);

	UFUNCTION(BlueprintCallable)
		void CreateGameSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName);

	UFUNCTION(BlueprintCallable)
		void FindGameSessions(int32 PlayerCount, int32 MaxSearchResult);

	UFUNCTION(BlueprintImplementableEvent)
		void StartLoadingScreen();

	UFUNCTION(BlueprintImplementableEvent)
		void StopLoadingScreen();

protected:
	virtual void OnCompleteSessionCreate(FName SessionName, bool result);
	virtual void OnCompleteSessionFinding(bool bIsSuccess);

	//virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* CreateSessionButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* UpdateSessionsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UScrollBox* FindedSessionScrollBox;

protected:
	const TSharedRef<FOnlineSessionSearch> LastSearchSettings = MakeShareable(new FOnlineSessionSearch);

private:
	FName SelectedGameLevel;

public:
	FOnCompleteCreationSession OnCompleteCreationSession;
	
};
