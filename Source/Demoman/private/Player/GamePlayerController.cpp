// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "Player/PlayerCharacter.h"
#include "UserWidget/GameUserWidget.h"
#include "UserWidget/SessionUserWidget.h"
#include "GameFramework/HUD.h"

AGamePlayerController::AGamePlayerController()
{
	ConstructorHelpers::FClassFinder<UGameUserWidget> WidgetClass(TEXT("/Game/Widgets/WD_Game"));
	if (WidgetClass.Succeeded())
	{
		GameWidgetClass = WidgetClass.Class;
	}
	ConstructorHelpers::FClassFinder<USessionUserWidget> gameLobbyWidgetClass(TEXT("/Game/Widgets/WD_GameLobby"));
	if (gameLobbyWidgetClass.Succeeded())
	{
		GameLobbyWidgetClass = gameLobbyWidgetClass.Class;
	}
}

void AGamePlayerController::UpdateGameWidget(int8 bombs, int8 power, float speed)
{
	if (!GameWidget) CreateGameWidget();
	GameWidget->UpdateWidget(bombs, power, speed);
}

void AGamePlayerController::CreateGameWidget()
{
	GameWidget = CreateWidget<UGameUserWidget>(GetWorld(), GameWidgetClass);
	checkf(GameWidget, TEXT("PlayerController cant create game widget"));

	GameWidget->AddToPlayerScreen(-1);
}

void AGamePlayerController::BeginPlay()
{
	if (IsLocalController())
	{
		SessionWidget = CreateWidget<USessionUserWidget>(this, GameLobbyWidgetClass);
		checkf(SessionWidget, TEXT("GameController cant create session widget!"));
		SessionWidget->AddToViewport(-1);

		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
}

void AGamePlayerController::OnPrepareGame()
{
	SessionWidget->RemoveFromViewport();
	
	if (!GameWidget) CreateGameWidget();
	GameWidget->StartGameAnimation();
	

	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("ClearedViewport"));
}

void AGamePlayerController::OnStartGame()
{
	SetInputMode(FInputModeGameOnly());

	APawn* tempPawn = GetPawn();
	checkf(tempPawn, TEXT("PlayerController missed pawn"));

	tempPawn->EnableInput(this);
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Game Started!"));
}

void AGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	APlayerCharacter* TemplPlayerCharacter = Cast<APlayerCharacter>(InPawn);
	checkf(TemplPlayerCharacter, TEXT("PlayerController missed player character"));
	TemplPlayerCharacter->UpdateGameWidget();
}
