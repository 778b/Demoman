// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

class UGameUserWidget;
class USessionUserWidget;

UCLASS()
class DEMOMAN_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGamePlayerController();

	void UpdateGameWidget(int8 bombs, int8 power, float speed);

	void BeginPlay() override;

public:
	UGameUserWidget* GameWidget;
	USessionUserWidget* SessionWidget;

protected:
	TSubclassOf<UGameUserWidget> GameWidgetClass;
	TSubclassOf<USessionUserWidget> GameLobbyWidgetClass;
};
