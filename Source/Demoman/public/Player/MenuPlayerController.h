// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class UGameUserWidget;
class USessionUserWidget;
class UFindedSessionWidget;


UCLASS()
class DEMOMAN_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMenuPlayerController();

	UClass* GetFindedSessionClass();
	UClass* GetSessionWidgetClass();

public:
	USessionUserWidget* SessionWidget;

protected:
	TSubclassOf<USessionUserWidget> GameLobbyWidgetClass;
	TSubclassOf<UFindedSessionWidget> FindedLobbyWidgetClass;
	TSubclassOf<UGameUserWidget> GameWidgetClass;

};
