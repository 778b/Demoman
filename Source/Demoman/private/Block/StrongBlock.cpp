// Fill out your copyright notice in the Description page of Project Settings.


#include "Block/StrongBlock.h"

#include "UObject/ConstructorHelpers.h"

AStrongBlock::AStrongBlock()
{
	BlockHealth = 6;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Material/Block/M_Strong.M_Strong"));
	if (MaterialAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MaterialAsset.Object);
	}
	SetChanceToDrop(15.f);
}
