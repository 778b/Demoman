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

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Widgets/WD_MainMenu"));
	if (WidgetClass.Succeeded())
	{
		MenuWidgetClass = WidgetClass.Class;
	}
}

void ADemomanMenuGameMode::BeginPlay()
{
	/* In simulate we can use default PlayerController ONLY
	 * Because EpicGames are genius :)
	*/ 
	APlayerController* TempController = GameState->PlayerArray[0]->GetOwner<APlayerController>();
	checkf(TempController, TEXT("MenuGameMode missed PlayerController!"));

	UUserWidget* TempWidget = CreateWidget<UUserWidget>(TempController, MenuWidgetClass);
	checkf(TempWidget, TEXT("MenuGameMode cant create widget menu!"));
	
	TempWidget->AddToViewport(-1);
	
	TempController->SetInputMode(FInputModeUIOnly());
	TempController->SetShowMouseCursor(true);
}
