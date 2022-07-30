// Fill out your copyright notice in the Description page of Project Settings.

#include "Slate/SMainMenu.h"

//#include ""
#include "UObject/ConstructorHelpers.h"
#include "SlateOptMacros.h"


#define LOCTEXT_NAMESPACE "SMainMenu"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenu::Construct(const FArguments& InArgs)
{

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(150.f, 250.f, 0.f, 0.f))
			[
				SNew(SButton)
				.ContentScale(FVector2D(200, 20))
				.OnClicked(this, &SMainMenu::OnButtonSingleplayer)
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SingleplayerButton", "Singleplayer"))
					.Font(MenuFont)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(150.f, 100.f, 0.f, 0.f))
			[
				SNew(SButton)
				.ContentScale(FVector2D(200, 20))
				.OnClicked(this, &SMainMenu::OnButtonMultiplayer)
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("MultiplayerButton", "Multiplayer"))
					.Font(MenuFont)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(150.f, 150.f, 0.f, 0.f))
			[
				SNew(SButton)
				.ContentScale(FVector2D(200, 20))
				.OnClicked(this, &SMainMenu::OnButtonSettings)
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SettingsButton", "Settings"))
					.Font(MenuFont)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(150.f, 150.f, 0.f, 0.f))
			[
				SNew(SButton)
				.ContentScale(FVector2D(200, 20))
				.OnClicked(this, &SMainMenu::OnButtonExit)
				.Content()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ExitButton", "Exit"))
				.Font(MenuFont)
			]
		]

	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FReply SMainMenu::OnButtonSingleplayer()
{
	return FReply::Handled();
}
FReply SMainMenu::OnButtonMultiplayer()
{
	return FReply::Handled();
}
FReply SMainMenu::OnButtonSettings()
{
	return FReply::Handled();
}
FReply SMainMenu::OnButtonExit()
{
	return FReply::Handled();
}

