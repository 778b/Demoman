// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MenuPlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "UserWidget/MainMenuWidget.h"
#include "UserWidget/FindedSessionWidget.h"
#include "UserWidget/SessionUserWidget.h"


AMenuPlayerController::AMenuPlayerController()
{
	ConstructorHelpers::FClassFinder<USessionUserWidget> gameLobbyWidgetClass(TEXT("/Game/Widgets/WD_GameLobby"));
	if (gameLobbyWidgetClass.Succeeded())
	{
		GameLobbyWidgetClass = gameLobbyWidgetClass.Class;
	}

	ConstructorHelpers::FClassFinder<UFindedSessionWidget> findedLobbyWidgetClass(TEXT("/Game/Widgets/WD_FindedSession"));
	if (findedLobbyWidgetClass.Succeeded())
	{
		FindedLobbyWidgetClass = findedLobbyWidgetClass.Class;
	}

	ConstructorHelpers::FClassFinder<UMainMenuWidget> menuWidgetClass(TEXT("/Game/Widgets/WD_MainMenu"));
	if (menuWidgetClass.Succeeded())
	{
		MenuWidgetClass = menuWidgetClass.Class;
	}
}

void AMenuPlayerController::BeginPlay()
{
	if (IsLocalController())
	{
		UMainMenuWidget* TempWidget = CreateWidget<UMainMenuWidget>(this, MenuWidgetClass);
		checkf(TempWidget, TEXT("MenuGameMode cant create widget menu!"));

		TempWidget->AddToViewport(-1);

		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
}

TSubclassOf<UUserWidget> AMenuPlayerController::GetFindedSessionClass()
{
	return FindedLobbyWidgetClass;
}

TSubclassOf<UUserWidget> AMenuPlayerController::GetSessionWidgetClass()
{
	return GameLobbyWidgetClass;
}
