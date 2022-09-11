// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CSNetworkSubsystem.h"

#include "Player/MenuPlayerController.h"
#include "UserWidget/SessionUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"


UCSNetworkSubsystem::UCSNetworkSubsystem()
{
	OnJoinSessionCompleteDelegate.BindUObject(this, &UCSNetworkSubsystem::OnJoinSessionCompleted);
	OnCreateSessionCompleteDelegate.BindUObject(this, &UCSNetworkSubsystem::OnCreateSessionCompleted);
	OnFindSessionsCompleteDelegate.BindUObject(this, &UCSNetworkSubsystem::OnFindSessionsCompleted);

}



/*
 *	// Generation Functions
 */


void UCSNetworkSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	if (!SessionPtr.IsValid())
	{
		OnJoinSessionCompleteEvent.Broadcast(NAME_None, EOnJoinSessionCompleteResult::SessionDoesNotExist);
		return;
	}
	SessionPtr->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionPtr->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult);
}

void UCSNetworkSubsystem::CreateSession(int32 NumPublicConnections, bool IsLAN, FString SessionName, FName LevelName)
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	if (!SessionPtr.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(NAME_None, false);
		return;
	}

	FOnlineSessionSettings CurrentSessionSettings = FOnlineSessionSettings();

	CurrentSessionSettings.NumPublicConnections = NumPublicConnections;
	CurrentSessionSettings.bIsLANMatch = IsLAN;
	CurrentSessionSettings.NumPrivateConnections = 0;
	CurrentSessionSettings.bAllowJoinViaPresence = true;
	CurrentSessionSettings.bIsDedicated = false;
	CurrentSessionSettings.bUsesPresence = true;
	CurrentSessionSettings.bShouldAdvertise = true;

	// Using FString constructor for avoiding implicit transform our string to FOnlineSessionSetting
	CurrentSessionSettings.Set(SETTING_GAMEMODE, FString(SessionName), EOnlineDataAdvertisementType::Type::ViaOnlineService);
	CurrentSessionSettings.Set(SETTING_MAPNAME, LevelName.ToString(), EOnlineDataAdvertisementType::Type::ViaOnlineService);

	SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionPtr->CreateSession(*localPlayer->GetPreferredUniqueNetId(), FName(SessionName), CurrentSessionSettings);
}

void UCSNetworkSubsystem::FindSessions(int32 PlayerCount, int32 MaxSearchResult)
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
	if (!SessionPtr.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(false);
		return;
	}

	SessionPtr->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	LastSearchSettings->MaxSearchResults = MaxSearchResult;
	LastSearchSettings->TimeoutInSeconds = 1000;

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionPtr->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSearchSettings);
}


/*
 *	// Binded Functions
 */



void UCSNetworkSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::FromInt(Result));
	if (Result == EOnJoinSessionCompleteResult::Type::Success)
	{
		AMenuPlayerController* TempController = GetWorld()->GetFirstPlayerController<AMenuPlayerController>();
		checkf(TempController, TEXT("NetworkSystem missed PlayerController!"));

		const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
		checkf(SessionPtr.IsValid(), TEXT("NetworkSystem missed SessionPtr"));

		LastSessionName = SessionName;

		FString ConnectionInfo;
		SessionPtr.Get()->GetResolvedConnectString(SessionName, ConnectionInfo);
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, ConnectionInfo);

		TempController->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
	}

	OnJoinSessionCompleteEvent.Broadcast(SessionName, Result);
}

void UCSNetworkSubsystem::OnCreateSessionCompleted(FName SessionName, bool Success)
{
	if (Success)
	{
		AMenuPlayerController* TempController = GetWorld()->GetFirstPlayerController<AMenuPlayerController>();
		checkf(TempController, TEXT("NetworkSystem missed PlayerController!"));

		const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetWorld());
		checkf(SessionPtr.IsValid(), TEXT("NetworkSystem missed SessionPtr"));

		FString MapName;
		check(SessionPtr->GetSessionSettings(SessionName)->Get(SETTING_MAPNAME, MapName));

		LastSessionName = SessionName;

		UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true, TEXT("?listen"));
	}
	OnCreateSessionCompleteEvent.Broadcast(SessionName, Success);
}

void UCSNetworkSubsystem::OnFindSessionsCompleted(bool Success)
{

	OnFindSessionsCompleteEvent.Broadcast(Success);
}

