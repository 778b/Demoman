// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemomanGameModeBase.h"

#include "Character/PlayerCharacter.h"

ADemomanGameModeBase::ADemomanGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}
