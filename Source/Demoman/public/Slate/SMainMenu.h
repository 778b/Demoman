// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"



class DEMOMAN_API SMainMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenu)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FSlateFontInfo MenuFont = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Black.ttf"), 28);

private:
	FReply OnButtonSingleplayer();

	FReply OnButtonMultiplayer();

	FReply OnButtonSettings();

	FReply OnButtonExit();

};
