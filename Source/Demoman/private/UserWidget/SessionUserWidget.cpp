// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget/SessionUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/PlayerUndecidedWidget.h"
#include "Game/CSNetworkSubsystem.h"
#include "Game/DemomanGameState.h"
#include "Player/GamePlayerController.h"
#include "Player/PlayerCharacter.h"

void USessionUserWidget::NativeConstruct()
{
	UCSNetworkSubsystem* NetworkSys = GetGameInstance()->GetSubsystem<UCSNetworkSubsystem>();
	checkf(NetworkSys, TEXT("SessionWidget missed NetworkSystem"));
	PublicSessionName = NetworkSys->LastSessionName;

	FGameModeEvents::GameModePostLoginEvent.AddUObject(this, &USessionUserWidget::OnPostLoginEvent);
	FGameModeEvents::GameModeLogoutEvent.AddUObject(this, &USessionUserWidget::OnLogoutEvent);

	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->OnUpdateWidgetDelegate.BindUObject(this, &USessionUserWidget::UpdateWidgetSettings);
	tempState->UpdateLobbyWidget();
	ConstructWidget();
}

void USessionUserWidget::OnJoinTeam(EPlayerLobbyTeam SelectedLobby)
{
	AGamePlayerState* tempPlayerState = Cast<AGamePlayerState>(
		GetOwningPlayer()->GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueId()));
	checkf(tempPlayerState, TEXT("SessionWidget missed PlayerState"));

	// Checking selected slot, maybe it is already busy
	for (APlayerState* tempPlayer : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedState = Cast<AGamePlayerState>(tempPlayer);
		if (SelectedLobby != Undecided && CastedState->PlayerLobbyState == SelectedLobby)
		{
			ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
			tempState->UpdateLobbyWidget();

			SetupPlayersInLobby();
			return;
		};
	}

	tempPlayerState->SetPlayerLobbyState(SelectedLobby);

	if (SelectedLobby != Undecided) BJoinUndecided->SetVisibility(ESlateVisibility::Visible);
	else BJoinUndecided->SetVisibility(ESlateVisibility::Hidden);

	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->UpdateLobbyWidget();

	SetupPlayersInLobby();
}

void USessionUserWidget::OnStartGame_Implementation()
{
	// Check all player states
	for (APlayerState* tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedPlayerState = Cast<AGamePlayerState>(tempPlayer);
		checkf(CastedPlayerState, TEXT("SessionWidget cant get GamePlayerState"));
		if (CastedPlayerState->PlayerLobbyState == Undecided) return;
	}
	
	// StartGame
	for (APlayerState*  tempPlayer : GetWorld()->GetGameState()->PlayerArray)
	{
		AController* tempController = tempPlayer->GetOwner<AController>();
		checkf(tempController, TEXT("SessionWidget cant get Controller"));
		check(GetWorld()->GetAuthGameMode());
		AActor* tempActor = GetWorld()->GetAuthGameMode()->ChoosePlayerStart(tempController);
		checkf(tempActor, TEXT("SessionWidget cant find PlayerStart"));
		APlayerCharacter* tempPlayerPawn = GetWorld()->SpawnActor<APlayerCharacter>(
			tempActor->GetActorLocation(), FRotator(0.f));
		tempActor->Destroy(true);
		checkf(tempPlayerPawn, TEXT("SessionWidget cant create PlayerPawn"));
		tempController->Possess(tempPlayerPawn);
		AGamePlayerController* tempCastedController = Cast<AGamePlayerController>(tempController);
		tempCastedController->OnStartGame();
	}
	
}

void USessionUserWidget::DrawDebugPlayers()
{
#ifdef UE_BUILD_DEVELOPMENT
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Begin"));
	for (APlayerState* tempPlayer : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedState = Cast<AGamePlayerState>(tempPlayer);
		FString TEmpString = CastedState->GetPlayerName() + " " + FString::FromInt(CastedState->PlayerLobbyState);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEmpString);
	}
#endif

}

FName USessionUserWidget::GetRoomName()
{
	return PublicSessionName;
}

void USessionUserWidget::SetupPlayersInLobby()
{
	SetupDafeultSettings();

	for (APlayerState* tempPlayer : GetOwningPlayer()->GetWorld()->GetGameState()->PlayerArray)
	{
		AGamePlayerState* CastedState = Cast<AGamePlayerState>(tempPlayer);

		switch (CastedState->GetPlayerLobbyState())
		{
		case Undecided:
		{
			UPlayerUndecidedWidget* tempWidget = CreateWidget<UPlayerUndecidedWidget>(GetOwningPlayer(), CastedState->PlayerUndecidedWidgetClass);
			tempWidget->PlayerName->SetText(FText::FromString(tempPlayer->GetPlayerName()));
			UndecidedScrollBox->AddChild(tempWidget);
		}
			break;
		case Red:
			BJoinRedColor->SetVisibility(ESlateVisibility::Collapsed);
			NameRedColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameRedColor->SetVisibility(ESlateVisibility::Visible);
			break;
		case Blue:
			BJoinBlueColor->SetVisibility(ESlateVisibility::Collapsed);
			NameBlueColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameBlueColor->SetVisibility(ESlateVisibility::Visible);
			break;
		case Green:
			BJoinGreenColor->SetVisibility(ESlateVisibility::Collapsed);
			NameGreenColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameGreenColor->SetVisibility(ESlateVisibility::Visible);
			break;
		case Yellow:
			BJoinYellowColor->SetVisibility(ESlateVisibility::Collapsed);
			NameYellowColor->SetText(FText::FromString(CastedState->GetPlayerName()));
			NameYellowColor->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}

void USessionUserWidget::SetupDafeultSettings()
{
	UndecidedScrollBox->ClearChildren();

	NameYellowColor->SetVisibility(ESlateVisibility::Collapsed);
	NameGreenColor->SetVisibility(ESlateVisibility::Collapsed);
	NameBlueColor->SetVisibility(ESlateVisibility::Collapsed);
	NameRedColor->SetVisibility(ESlateVisibility::Collapsed);

	BJoinGreenColor->SetVisibility(ESlateVisibility::Visible);
	BJoinYellowColor->SetVisibility(ESlateVisibility::Visible);
	BJoinBlueColor->SetVisibility(ESlateVisibility::Visible);
	BJoinRedColor->SetVisibility(ESlateVisibility::Visible);
}

void USessionUserWidget::UpdateWidgetSettings()
{
	SetupPlayersInLobby();
}

void USessionUserWidget::OnPostLoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	SetupPlayersInLobby();
}

void USessionUserWidget::OnLogoutEvent(AGameModeBase* GameMode, AController* Exiting)
{
	SetupPlayersInLobby();
}
