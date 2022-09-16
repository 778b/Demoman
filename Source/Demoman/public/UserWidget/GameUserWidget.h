// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUserWidget.generated.h"


class UTextBlock;

UCLASS()
class DEMOMAN_API UGameUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UGameUserWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateWidget(int8 bombs, int8 power, float speed);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		void StartGameAnimation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* BombsCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* BombsPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PlayerSpeed;

};
