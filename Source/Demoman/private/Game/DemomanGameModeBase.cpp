// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/DemomanGameModeBase.h"

#include "Player/PlayerCharacter.h"

ADemomanGameModeBase::ADemomanGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}
