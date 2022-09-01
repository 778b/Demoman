// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemomanMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"
#include "Player/MenuPlayerController.h"
#include "Player/PlayerHUD.h"


ADemomanMenuGameMode::ADemomanMenuGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = APlayerHUD::StaticClass();

	checkf(PlayerControllerClass && HUDClass, TEXT("MenuGamemode cant create static class!"));


}

void ADemomanMenuGameMode::BeginPlay()
{

}
