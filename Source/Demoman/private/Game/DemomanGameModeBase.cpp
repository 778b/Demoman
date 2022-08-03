// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/DemomanGameModeBase.h"

#include "Player/PlayerCharacter.h"
#include "Player/PlayerHUD.h"
#include "Player/GamePlayerState.h"
#include "Player/GamePlayerController.h"

ADemomanGameModeBase::ADemomanGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	PlayerControllerClass = AGamePlayerController::StaticClass();
	PlayerStateClass = AGamePlayerState::StaticClass();
}
