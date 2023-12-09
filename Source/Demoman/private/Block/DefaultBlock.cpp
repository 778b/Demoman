// Fill out your copyright notice in the Description page of Project Settings.

#include "Block/DefaultBlock.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"

ADefaultBlock::ADefaultBlock()
{
	BlockHealth = 1;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Material/Block/M_Default.M_Default"));
	if (MaterialAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MaterialAsset.Object);
	}
}
