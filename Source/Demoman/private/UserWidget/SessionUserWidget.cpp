// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget/SessionUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CSNetworkSubsystem.h"

void USessionUserWidget::NativeConstruct()
{
	// Debug todo delete
	// must relocated to NetworkSystem
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	checkf(SessionPtr.IsValid(), TEXT("Session is not valid!"));

	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("FindedWidget missed NetworkSystem"));
	PublicSessionName = NetworkSys->LastSessionName;

	SessionPtr->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(
		this , &USessionUserWidget::OnRegisterPlayersCompleted));

	ConstructWidget();
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
	//for (APlayerState*& Player : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Player->GetPlayerName());

	//}
	//for (ULocalPlayer* Player : GetOwningPlayer()->GetWorld()->GetGameInstance()->GetLocalPlayers())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Player->GetNickname());
	//}
}

FName USessionUserWidget::GetRoomName()
{
	return PublicSessionName;
}

void USessionUserWidget::OnRegisterPlayersCompleted(FName ServerName, const TArray<FUniqueNetIdRef>& Players, bool Success)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Session was registered new Player");

	// todo Players
}
