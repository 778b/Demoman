// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MenuPlayerController.h"

#include "UObject/ConstructorHelpers.h"
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
}

UClass* AMenuPlayerController::GetFindedSessionClass()
{
	return FindedLobbyWidgetClass;
}

UClass* AMenuPlayerController::GetSessionWidgetClass()
{
	return GameLobbyWidgetClass;
}
