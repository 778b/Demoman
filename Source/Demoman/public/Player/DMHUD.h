// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DMHUD.generated.h"

/**
 * 
 */
UCLASS()
class DEMOMAN_API ADMHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void DrawHUD() override;
};
