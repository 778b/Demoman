// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MenuPlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "UserWidget/MainMenuWidget.h"
#include "UserWidget/FindedSessionWidget.h"



AMenuPlayerController::AMenuPlayerController()
{
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
		checkf(TempWidget, TEXT("MenuController cant create widget menu!"));

		TempWidget->AddToViewport(-1);

		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
}

TSubclassOf<UUserWidget> AMenuPlayerController::GetFindedSessionClass()
{
	return FindedLobbyWidgetClass;
}

