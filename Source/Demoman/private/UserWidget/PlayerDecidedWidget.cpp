// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PlayerDecidedWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/DemomanGameState.h"
#include "Player/GamePlayerState.h"

void UPlayerDecidedWidget::NativeConstruct()
{
	switch(PlayerLobbyColor)
	{
	case EPlayerLobbyTeam::Undecided:
		break;
	case EPlayerLobbyTeam::Red:
		ColorSlot->SetBrushTintColor(RedColor);
		break;
	case EPlayerLobbyTeam::Blue:
		ColorSlot->SetBrushTintColor(BlueColor);
		break;
	case EPlayerLobbyTeam::Yellow:
		ColorSlot->SetBrushTintColor(YellowColor);
		break;
	case EPlayerLobbyTeam::Green:
		ColorSlot->SetBrushTintColor(GreenColor);
		break;
	}


}

void UPlayerDecidedWidget::SetupSettings(AGamePlayerState* Player)
{
	if (Player->PlayerLobbyRole == Admin)
	{
		BAddBotToSlot->SetVisibility(ESlateVisibility::Visible);
		BKickSlot->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerDecidedWidget::OnJoinTeam(EPlayerLobbyTeam SelectedLobby)
{
	AGamePlayerState* tempPlayerState = Cast<AGamePlayerState>(
		GetOwningPlayer()->GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueId()));

	// Player press button before join 
	if (!tempPlayerState) return;

	tempPlayerState->SetPlayerLobbyState(SelectedLobby);
	//SetupPlayersInLobby();

	if (SelectedLobby != Undecided) BJoinSlot->SetVisibility(ESlateVisibility::Visible);
	else BJoinSlot->SetVisibility(ESlateVisibility::Hidden);

	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	tempState->UpdateLobbyWidget();
}

void UPlayerDecidedWidget::AddBotToGame_Implementation()
{
}

void UPlayerDecidedWidget::RemovePlayerFromGame_Implementation()
{
}