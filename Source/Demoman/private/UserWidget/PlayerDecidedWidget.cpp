// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PlayerDecidedWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/DemomanGameState.h"
#include "Player/GamePlayerState.h"


void UPlayerDecidedWidget::NativeConstruct()
{
	NameSlot->SetVisibility(ESlateVisibility::Collapsed);
	BAddBotToSlot->SetVisibility(ESlateVisibility::Collapsed);
	BKickSlot->SetVisibility(ESlateVisibility::Collapsed);

	BJoinSlot->OnClicked.AddDynamic(this, &UPlayerDecidedWidget::OnClickedJoinButton);
}

void UPlayerDecidedWidget::SetupSettings(AGamePlayerState* Player, EPlayerLobbyRole OwnerRole)
{

	// Slot is empty
	if (Player == nullptr)
	{
		PlayerLobbyColor = Undecided;
		if(NextSlotColor > 3)
		{
			NextSlotColor = 0;
		}
		ColorSlot->SetBrushTintColor(SlotColors[NextSlotColor]);

		ColorSlot->SetVisibility(ESlateVisibility::Visible);
		BJoinSlot->SetVisibility(ESlateVisibility::Visible);

		switch (NextSlotColor)
		{
		case 0:
			PlayerLobbyColor = Red;
			break;
		case 1:
			PlayerLobbyColor = Blue;
			break;
		case 2:
			PlayerLobbyColor = Green;
			break;
		case 3:
			PlayerLobbyColor = Yellow;
			break;
		}
	}
	// Slot containing the player
	else
	{
		switch (Player->PlayerLobbyState)
		{
		case EPlayerLobbyTeam::Undecided:
			break;
		case EPlayerLobbyTeam::Red:
			ColorSlot->SetBrushTintColor(SlotColors[0]);
			break;
		case EPlayerLobbyTeam::Blue:
			ColorSlot->SetBrushTintColor(SlotColors[1]);
			break;
		case EPlayerLobbyTeam::Yellow:
			ColorSlot->SetBrushTintColor(SlotColors[2]);
			break;
		case EPlayerLobbyTeam::Green:
			ColorSlot->SetBrushTintColor(SlotColors[3]);
			break;
		}
		BJoinSlot->SetVisibility(ESlateVisibility::Collapsed);
		NameSlot->SetText(FText::FromString(Player->GetPlayerName()));
		NameSlot->SetVisibility(ESlateVisibility::Visible);

		// @todo delete
		FString OutputString = NameSlot->GetText().ToString() + " " + FString::FromInt((int32)NameSlot->GetVisibility());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, OutputString);
		}
		// end delete @todo


		if (OwnerRole == Admin)
		{
			BKickSlot->SetVisibility(ESlateVisibility::Visible);
			BAddBotToSlot->SetVisibility(ESlateVisibility::Visible);
		}
	}

	

	NextSlotColor++;
}

void UPlayerDecidedWidget::OnJoinTeam(EPlayerLobbyTeam SelectedLobby)
{
	// Client only function all settings get from our local player
	AGamePlayerState* tempPlayerState = Cast<AGamePlayerState>(
		GetOwningPlayer()->GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueId()));

	// Player press button before join 
	if (!tempPlayerState) return;

	tempPlayerState->SetPlayerLobbyState(SelectedLobby);
	// SetupPlayersInLobby();

	//if (SelectedLobby != Undecided)
	//NameSlot->SetText(FText::FromString(tempPlayerState->GetPlayerName()));
	//NameSlot->SetVisibility(ESlateVisibility::Visible);
	//BJoinSlot->SetVisibility(ESlateVisibility::Collapsed);
	//else BJoinSlot->SetVisibility(ESlateVisibility::Hidden);

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

void UPlayerDecidedWidget::OnClickedJoinButton()
{
	OnJoinTeam(PlayerLobbyColor);
}

void UPlayerDecidedWidget::OnClickedKickButton()
{
}

void UPlayerDecidedWidget::OnClickedAddBotButton()
{
}
