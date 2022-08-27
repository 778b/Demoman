// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget/SessionUserWidget.h"

#include "Kismet/GameplayStatics.h"

void USessionUserWidget::OnJoinSession(APlayerController* JoinedPlayer)
{
}

void USessionUserWidget::OnLeaveSession(APlayerController* LeavedPlayer)
{
}

void USessionUserWidget::OnJoinTeam(FColor SelectedColor, APlayerController* Player)
{
}

void USessionUserWidget::OnJoinUndecided(APlayerController* Leaver)
{
}

void USessionUserWidget::OnSelectNewMap()
{
}

void USessionUserWidget::OnStartGame()
{
	//UGameplayStatics::OpenLevel()
}
