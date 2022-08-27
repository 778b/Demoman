// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "UserWidget/GameUserWidget.h"
#include "GameFramework/HUD.h"

AGamePlayerController::AGamePlayerController()
{
	ConstructorHelpers::FClassFinder<UGameUserWidget> WidgetClass(TEXT("/Game/Widgets/WD_Game"));
	if (WidgetClass.Succeeded())
	{
		GameWidgetClass = WidgetClass.Class;
	}
}

void AGamePlayerController::UpdateGameWidget(int8 bombs, int8 power, float speed)
{
	if (!GameWidget)
	{
		GameWidget = CreateWidget<UGameUserWidget>(this, GameWidgetClass);
		GameWidget->AddToPlayerScreen(-1);
	}
	if (GameWidget) GameWidget->UpdateWidget(bombs, power, speed);
}