// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/SpeedItems.h"

#include "Character/BaseCharacter.h"

void ASpeedItems::Consume(ABaseCharacter* InstigatorChar)
{
	InstigatorChar->AddMovementSpeed(100.f);
}
