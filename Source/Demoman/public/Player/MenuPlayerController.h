// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class UGameUserWidget;
class UMainMenuWidget;
class USessionUserWidget;
class UFindedSessionWidget;


UCLASS()
class DEMOMAN_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMenuPlayerController();

	void BeginPlay() override;

	TSubclassOf<UUserWidget> GetFindedSessionClass();

protected:
	TSubclassOf<UFindedSessionWidget> FindedLobbyWidgetClass;
	TSubclassOf<UGameUserWidget> GameWidgetClass;
	TSubclassOf<UMainMenuWidget> MenuWidgetClass;

};
