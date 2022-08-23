// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemomanMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"


ADemomanMenuGameMode::ADemomanMenuGameMode()
{
	DefaultPawnClass = nullptr;

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Widgets/WD_MainMenu"));
	if (WidgetClass.Succeeded())
	{
		MenuWidgetClass = WidgetClass.Class;
	}
}

void ADemomanMenuGameMode::BeginPlay()
{
	APlayerController* TempController = Cast<APlayerController>(GameState->PlayerArray[0]->GetOwner());
	check(TempController);

	UUserWidget* TempWidget = CreateWidget<UUserWidget>(TempController, MenuWidgetClass);
	check(TempWidget);
	
	TempWidget->AddToViewport(-1);
	
	TempController->SetInputMode(FInputModeUIOnly());
	TempController->SetShowMouseCursor(true);
}
