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
	DefaultPawnClass = nullptr;
	HUDClass = APlayerHUD::StaticClass();
	PlayerControllerClass = AGamePlayerController::StaticClass();
	PlayerStateClass = AGamePlayerState::StaticClass();
	GameSessionClass = ADemomanGameSession::StaticClass();

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Widgets/WD_MainMenu"));
	if (WidgetClass.Succeeded())
	{
		MenuWidgetClass = WidgetClass.Class;
	}
}


void ADemomanGameModeBase::BeginPlay()
{
	APlayerController* TempController = Cast<APlayerController>(GameState->PlayerArray[0]->GetOwner());
	check(TempController);

	UUserWidget* TempWidget = CreateWidget<UUserWidget>(TempController, MenuWidgetClass);
	check(TempWidget);

	TempWidget->AddToViewport(-1);

	TempController->SetInputMode(FInputModeUIOnly());
	TempController->SetShowMouseCursor(true);
}