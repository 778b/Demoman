// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

class UGameUserWidget;


UCLASS()
class DEMOMAN_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGamePlayerController();

	void UpdateGameWidget(int8 bombs, int8 power, float speed);

public:
	UGameUserWidget* GameWidget;

protected:
	TSubclassOf<UGameUserWidget> GameWidgetClass;
};
