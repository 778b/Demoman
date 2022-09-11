// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUndecidedWidget.generated.h"

class UTextBlock;


UCLASS()
class DEMOMAN_API UPlayerUndecidedWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* PlayerName;
};
