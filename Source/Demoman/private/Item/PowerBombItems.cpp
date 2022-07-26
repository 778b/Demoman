// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PowerBombItems.h"

#include "Character/BaseCharacter.h"

void APowerBombItems::Consume(ABaseCharacter* InstigatorChar)
{
	InstigatorChar->AddBombsPower(1);
}
