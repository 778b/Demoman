// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickableItems.h"
#include "SpeedItems.generated.h"

/**
 * 
 */
UCLASS()
class DEMOMAN_API ASpeedItems : public ABasePickableItems
{
	GENERATED_BODY()
	
public:
	void Consume(class ABaseCharacter* InstigatorChar) override;
};
