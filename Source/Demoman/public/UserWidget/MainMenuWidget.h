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
		void CreateGameSession(int32 NumPublicConnections, bool IsLAN, FString SessionName);

	UFUNCTION(BlueprintCallable)
		void FindGameSessions(int32 PlayerCount, int32 MaxSearchResult);

protected:
	void OnCompleteSessionCreate(FName SessionName, bool result);
	void OnCompleteSessionFinding(bool bIsSuccess);

protected:
	UPROPERTY(meta = (BindWidget))
		UButton* CreateSessionButton;
	UPROPERTY(meta = (BindWidget))
		UButton* UpdateSessionsButton;
	UPROPERTY(meta = (BindWidget))
		UScrollBox* FindedSessionScrollBox;

protected:
	TSubclassOf<UUserWidget> GameLobbyWidgetClass;
	TSubclassOf<UFindedSessionWidget> FindedLobbyWidgetClass;
	const TSharedRef<FOnlineSessionSearch> LastSearchSettings = MakeShareable(new FOnlineSessionSearch);

public:
	FOnCompleteCreationSession OnCompleteCreationSession;
	
};
