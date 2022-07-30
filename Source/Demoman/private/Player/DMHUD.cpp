// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMHUD.h"

#include "Slate/SMainMenu.h"

void ADMHUD::DrawHUD()
{
	if (GEngine) GEngine->GameViewport->AddViewportWidgetContent(SNew(SMainMenu));
}
