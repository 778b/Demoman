// Fill out your copyright notice in the Description page of Project Settings.

#include "Block/DefaultBlock.h"

#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstance.h"

ADefaultBlock::ADefaultBlock()
{
	BlockHealth = 1;
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MeshAsset(TEXT("/Game/Material/Block/MI_Default.MI_Default"));
	if (MeshAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MeshAsset.Object);
	}
}
