// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget/SessionUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

void USessionUserWidget::NativeConstruct()
{
	// Debug todo delete
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	checkf(SessionPtr.IsValid(), TEXT("Session is not valid!"));

	FString ConnectInfo;
	SessionPtr->GetResolvedConnectString(PublicSessionName, ConnectInfo);
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, ConnectInfo);

	SessionPtr->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(
		this , &USessionUserWidget::OnRegisterPlayersCompleted));
}

void USessionUserWidget::OnJoinSession(APlayerController* JoinedPlayer)
{
}

void USessionUserWidget::OnLeaveSession(APlayerController* LeavedPlayer)
{
}

void USessionUserWidget::OnJoinTeam(FColor SelectedColor, APlayerController* Player)
{
}

void USessionUserWidget::OnJoinUndecided(APlayerController* Leaver)
{
}

void USessionUserWidget::OnSelectNewMap()
{
}

void USessionUserWidget::OnStartGame()
{
	//UGameplayStatics::OpenLevel()
}

void USessionUserWidget::DrawDebugPlayers()
{
	for (APlayerState*& Player : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Player->GetPlayerName());

	}
	for (ULocalPlayer* Player : GetOwningPlayer()->GetWorld()->GetGameInstance()->GetLocalPlayers())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Player->GetNickname());
	}
}

void USessionUserWidget::OnRegisterPlayersCompleted(FName ServerName, const TArray<FUniqueNetIdRef>& Players, bool Success)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Session was registered new Player");
}
