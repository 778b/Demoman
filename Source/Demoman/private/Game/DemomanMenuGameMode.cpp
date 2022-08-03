// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemomanMenuGameMode.h"

#include "Player/PlayerHUD.h"
#include "Player/MenuPlayerController.h"

ADemomanMenuGameMode::ADemomanMenuGameMode()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
}
