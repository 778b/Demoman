// Fill out your copyright notice in the Description page of Project Settings.


#include "Block/IndestructibleBlock.h"

#include "UObject/ConstructorHelpers.h"

AIndestructibleBlock::AIndestructibleBlock()
{
	BlockHealth = -1;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshAsset(TEXT("/Game/Material/Block/MI_Indestructible.MI_Indestructible"));
	if (MeshAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MeshAsset.Object);
	}
}
