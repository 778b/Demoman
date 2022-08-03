// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/GameUserWidget.h"

#include "Components/TextBlock.h"

void UGameUserWidget::UpdateWidget(int8 bombs, int8 power, float speed)
{
	if (BombsCount)		BombsCount->SetText(FText::FromString("Bombs: " + FString::FromInt(bombs)));
	if (BombsPower)		BombsPower->SetText(FText::FromString("Power: " + FString::FromInt(power)));
	if (PlayerSpeed)	PlayerSpeed->SetText(FText::FromString("Speed: " + FString::SanitizeFloat(speed)));
}

void UGameUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

}
