// Fill out your copyright notice in the Description page of Project Settings.


#include "Block/IndestructibleBlock.h"

AIndestructibleBlock::AIndestructibleBlock()
{
	BlockHealth = -1;
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MeshAsset(TEXT("/Game/Material/Block/MI_Indestructible.MI_Indestructible"));
	if (MeshAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MeshAsset.Object);
	}
}
