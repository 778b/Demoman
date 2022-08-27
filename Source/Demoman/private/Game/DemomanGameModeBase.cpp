// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/DemomanGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Game/DemomanGameSession.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ConstructorHelpers.h"
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
	GameSessionClass = ADemomanGameSession::StaticClass();
}

