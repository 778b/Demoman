// Fill out your copyright notice in the Description page of Project Settings.


#include "Block/StrongBlock.h"

#include "UObject/ConstructorHelpers.h"

AStrongBlock::AStrongBlock()
{
	BlockHealth = 6;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshAsset(TEXT("/Game/Material/Block/MI_Strong.MI_Strong"));
	if (MeshAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MeshAsset.Object);
	}
	SetChanceToDrop(15.f);
}
