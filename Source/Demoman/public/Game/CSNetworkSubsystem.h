// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "CSNetworkSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FNSOnJoinSessionComplete, FName, EOnJoinSessionCompleteResult::Type Result);
DECLARE_MULTICAST_DELEGATE_TwoParams(FNSOnCreateSessionComplete, FName, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FNSOnFindSessionsComplete, bool);

/*	Player's support network system
 *	instanced once for client
 *	NOT REPLICATED
 */
UCLASS()
class DEMOMAN_API UCSNetworkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCSNetworkSubsystem();

	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void CreateSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName);
	void FindSessions(int32 PlayerCount, int32 MaxSearchResult);

	FOnlineSessionSearch GetLastSearchResult() { return LastSearchSettings.Get(); };


protected:
	virtual void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnCreateSessionCompleted(FName SessionName, bool Success);
	virtual void OnFindSessionsCompleted(bool Success);


public:
	FNSOnJoinSessionComplete	OnJoinSessionCompleteEvent;
	FNSOnCreateSessionComplete	OnCreateSessionCompleteEvent;
	FNSOnFindSessionsComplete	OnFindSessionsCompleteEvent;

	FName LastSessionName;



protected:
	const TSharedRef<FOnlineSessionSearch> LastSearchSettings = MakeShareable(new FOnlineSessionSearch);

private:
	FOnJoinSessionCompleteDelegate		OnJoinSessionCompleteDelegate;
	FOnCreateSessionCompleteDelegate	OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate		OnFindSessionsCompleteDelegate;
};
