// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemomanGameModeBase.generated.h"


class UUserWidget;


UCLASS()
class DEMOMAN_API ADemomanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADemomanGameModeBase();

	void BeginPlay() override;

protected:
	TSubclassOf<UUserWidget> MenuWidgetClass;
};
