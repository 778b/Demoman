// Fill out your copyright notice in the Description page of Project Settings.


#include "Block/SupriseBlock.h"

#include "UObject/ConstructorHelpers.h"

ASupriseBlock::ASupriseBlock()
{
	BlockHealth = 1;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshAsset(TEXT("/Game/Material/Block/MI_Surprise.MI_Surprise"));
	if (MeshAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MeshAsset.Object);
	}
	SetChanceToDrop(85.f);
}
