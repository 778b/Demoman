// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PlayerDecidedWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/GameInstance.h"
#include "Game/DemomanGameState.h"
#include "Player/GamePlayerState.h"


void UPlayerDecidedWidget::NativeConstruct()
{
	BJoinSlot->OnClicked.AddDynamic(this, &UPlayerDecidedWidget::OnClickedJoinButton);
}

bool UPlayerDecidedWidget::Initialize()
{
	bool superResult = Super::Initialize();
	NameSlot->SetVisibility(ESlateVisibility::Collapsed);
	BAddBotToSlot->SetVisibility(ESlateVisibility::Collapsed);
	BKickSlot->SetVisibility(ESlateVisibility::Collapsed);

	return superResult;
}

void UPlayerDecidedWidget::SetupSettings(AGamePlayerState* Player, EPlayerLobbyRole OwnerRole)
{
	// Slot is empty
	if (Player == nullptr)
	{
		// Getting first free color
		for (int32 i = 0; i < EPlayerLobbyTeam::Max; ++i)
		{
			PlayerLobbyColor = static_cast<EPlayerLobbyTeam>(i);
			ColorSlot->SetBrushTintColor(SlotColors[i]);
			break;
		}
		ColorSlot->SetVisibility(ESlateVisibility::Visible);
		BJoinSlot->SetVisibility(ESlateVisibility::Visible);
	}
	// Slot containing the player
	else
	{
		switch (Player->GetPlayerLobbyState())
		{
		case EPlayerLobbyTeam::Undecided:
			break;
		case EPlayerLobbyTeam::Red:
			ColorSlot->SetBrushTintColor(SlotColors[0]);
			break;
		case EPlayerLobbyTeam::Blue:
			ColorSlot->SetBrushTintColor(SlotColors[1]);
			break;
		case EPlayerLobbyTeam::Green:
			ColorSlot->SetBrushTintColor(SlotColors[2]);
			break;
		case EPlayerLobbyTeam::Yellow:
			ColorSlot->SetBrushTintColor(SlotColors[3]);
			break;
		}
		PlayerLobbyColor = Player->GetPlayerLobbyState();
		BJoinSlot->SetVisibility(ESlateVisibility::Collapsed);
		NameSlot->SetText(FText::FromString(Player->GetPlayerName()));
		NameSlot->SetVisibility(ESlateVisibility::Visible);

		if (OwnerRole == Admin)
		{
			BKickSlot->SetVisibility(ESlateVisibility::Visible);
			BAddBotToSlot->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

const void UPlayerDecidedWidget::UpdateColor()
{
	switch (PlayerLobbyColor)
	{
	case EPlayerLobbyTeam::Undecided:
		break;
	case EPlayerLobbyTeam::Red:
		ColorSlot->SetBrushTintColor(SlotColors[0]);
		break;
	case EPlayerLobbyTeam::Blue:
		ColorSlot->SetBrushTintColor(SlotColors[1]);
		break;
	case EPlayerLobbyTeam::Green:
		ColorSlot->SetBrushTintColor(SlotColors[2]);
		break;
	case EPlayerLobbyTeam::Yellow:
		ColorSlot->SetBrushTintColor(SlotColors[3]);
		break;
	}
}

void UPlayerDecidedWidget::OnJoinTeam(EPlayerLobbyTeam SelectedLobby)
{
	// Client only function all settings get from our local player
	AGamePlayerState* tempPlayerState = Cast<AGamePlayerState>(
		GetOwningPlayer()->GetWorld()->GetGameState()->GetPlayerStateFromUniqueNetId(
			GetGameInstance()->GetPrimaryPlayerUniqueIdRepl()));

	// Player press button before join 
	if (!tempPlayerState) return;

	tempPlayerState->SetPlayerLobbyState(SelectedLobby);

	ADemomanGameState* tempState = Cast<ADemomanGameState>(GetWorld()->GetGameState());
	checkf(tempState, TEXT("SessionWidget missed GameState"));
	//tempState->Server_UpdateWidget();
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
	FOnChangeTeamDelegate.ExecuteIfBound(PlayerLobbyColor);
}

void UPlayerDecidedWidget::OnClickedKickButton()
{
}

void UPlayerDecidedWidget::OnClickedAddBotButton()
{
}
