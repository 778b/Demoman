// Fill out your copyright notice in the Description page of Project Settings.


#include "Block/SupriseBlock.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

ASupriseBlock::ASupriseBlock()
{
	BlockHealth = 1;
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Material/Block/M_Surprise.M_Surprise"));
	if (MaterialAsset.Succeeded())
	{
		BlockMesh->SetMaterial(0, MaterialAsset.Object);
	}
	SetChanceToDrop(85.f);
}
