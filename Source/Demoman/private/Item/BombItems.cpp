// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/BombItems.h"

#include "Character/BaseCharacter.h"

void ABombItems::Consume(ABaseCharacter* InstigatorChar)
{
	InstigatorChar->AddBombsCount(1);
}
