// Fill out your copyright notice in the Description page of Project Settings.


#include "Block/IndestructibleBlock.h"

#include "UObject/ConstructorHelpers.h"

AIndestructibleBlock::AIndestructibleBlock()
{
	BlockHealth = -1;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Material/Block/M_Indestructible.M_Indestructible"));
	if (MaterialAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MaterialAsset.Object);
	}
}
