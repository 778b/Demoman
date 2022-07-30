// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickableItems.h"
#include "PowerBombItems.generated.h"

/**
 * 
 */
UCLASS()
class DEMOMAN_API APowerBombItems : public ABasePickableItems
{
	GENERATED_BODY()

public:
	APowerBombItems();

	void Consume(class ABaseCharacter* InstigatorChar) override;
};
