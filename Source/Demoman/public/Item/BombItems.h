// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickableItems.h"
#include "BombItems.generated.h"

/**
 * 
 */
UCLASS()
class DEMOMAN_API ABombItems : public ABasePickableItems
{
	GENERATED_BODY()
	
public:
	ABombItems();

	void Consume(class ABaseCharacter* InstigatorChar) override;
};
