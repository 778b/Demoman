// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "CSNetworkSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FNSOnJoinSessionComplete, FName, EOnJoinSessionCompleteResult::Type Result);
DECLARE_MULTICAST_DELEGATE_TwoParams(FNSOnStartSessionComplete, FName, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FNSOnDestroySessionComplete, FName, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FNSOnCreateSessionComplete, FName, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FNSOnFindSessionsComplete, bool);



/**
 *	Player's support network system
 *	Singletone, created once for game client
 *	NOT REPLICATED
 */
UCLASS()
class DEMOMAN_API UCSNetworkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCSNetworkSubsystem();

	/**
	 *	Join Session simple function
	 *	@param SessionResult the struct from FindSessions()
	 *	Broadcasting in OnJoinSessionCompleteEvent
	 */
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	/**
	 *	Start Session simple function, execute after create the session
	 *	@param SessionName the name can taken from LastSessionName
	 *	Broadcasting in OnStartSessionCompleteEvent
	 */
	void StartSession(FName SessionName);

	/**
	 *	Destory Session simple function
	 *	@param SessionName the name can taken from LastSessionName
	 *	Broadcasting in OnDestroySessionCompleteEvent
	 */
	void DestroySession(FName SessionName);

	/**
	 *	Create Session simple function
	 *	@param NumPublicConnections the count of max players can join
	 *	@param IsLan used for determining local or internet server
	 *	@param SessionName name of your server 
	 *	@param LevelName name of chosen level
	 *	Broadcasting in OnStartSessionCompleteEvent
	 */
	void CreateSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName);

	/**
	 *	Find Sessions simple function
	 *	@param MaxSearchResult for max session on the screen
	 *	@param WantLan for determining local or internet servers
	 *	Broadcasting in OnFindSessionsCompleteEvent
	 */
	void FindSessions(int32 MaxSearchResult, bool WantLan = false);

	// Default getter for OnlineSessionSearch
	FOnlineSessionSearch& GetLastSearchResult() { return LastSearchSettings.Get(); };

protected:
	/**
	 *	Executed on complete session join
	 *	Executing OnJoinSessionCompleteEvent
	 */
	virtual void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	 *	Executed on complete session start
	 *	Executing OnStartSessionCompleteEvent
	 */
	virtual void OnStartSessionCompleted(FName SessionName, bool Success);

	/**
	 *	Executed on complete destroy session
	 *	Executing OnDestroySessionCompleteEvent
	 */
	virtual void OnDestroySessionCompleted(FName SessionName, bool Success);

	/**
	 *	Executed on complete session create
	 *	Executing OnCreateSessionCompleteEvent
	 */
	virtual void OnCreateSessionCompleted(FName SessionName, bool Success);

	/**
	 *	Executed on complete sessions find
	 *	Executing OnFindSessionsCompleteEvent
	 */
	virtual void OnFindSessionsCompleted(bool Success);

public:
	/**
	 *	Multicast delegate, broadcasting when join session complete
	 *	@param FName SessionName
	 *	@param EOnJoinSessionCompleteResult::Type Result
	 */
	FNSOnJoinSessionComplete	OnJoinSessionCompleteEvent;

	/**
	 *	Multicast delegate, broadcasting when create session complete
	 *	@param FName SessionName
	 *	@param bool Result
	 */
	FNSOnCreateSessionComplete	OnCreateSessionCompleteEvent;

	/**
	 *	Multicast delegate, broadcasting when destroy session complete
	 *	@param FName SessionName
	 *	@param bool Result
	 */
	FNSOnDestroySessionComplete OnDestroySessionCompleteEvent;

	/**
	 *	Multicast delegate, broadcasting when find sessions complete
	 *	@param bool Result
	 */
	FNSOnFindSessionsComplete	OnFindSessionsCompleteEvent;

	/**
	 *	Multicast delegate, broadcasting when start session complete
	 *	@param FName SessionName
	 *	@param bool Result
	 */
	FNSOnStartSessionComplete	OnStartSessionCompleteEvent;

	// Name that updated on JoinSession and CreateSession;
	FName LastSessionName;

protected:
	const TSharedRef<FOnlineSessionSearch> LastSearchSettings = MakeShareable(new FOnlineSessionSearch);

private:
	// private Delegate which executing OnJoinSessionCompleteEvent
	FOnJoinSessionCompleteDelegate		OnJoinSessionCompleteDelegate;

	// private Delegate which executing OnStartSessionCompleteEvent
	FOnStartSessionCompleteDelegate		OnStartSessionCompleteDelegate;

	// private Delegate which executing OnCreateSessionCompleteEvent
	FOnCreateSessionCompleteDelegate	OnCreateSessionCompleteDelegate;

	// private Delegate which executing OnFindSessionsCompleteEvent
	FOnFindSessionsCompleteDelegate		OnFindSessionsCompleteDelegate;

	// private Delegate which executing OnDestroySessionCompleteEvent
	FOnDestroySessionCompleteDelegate	OnDestroySessionCompleteDelegate;
};
