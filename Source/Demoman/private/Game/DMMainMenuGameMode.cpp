// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DMMainMenuGameMode.h"

#include "Player/DMHUD.h"

ADMMainMenuGameMode::ADMMainMenuGameMode()
{
	DefaultPawnClass = APawn::StaticClass();
	HUDClass = ADMHUD::StaticClass();
}
