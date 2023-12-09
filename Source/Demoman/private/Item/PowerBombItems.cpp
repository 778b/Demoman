// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PowerBombItems.h"

#include "ItemsFactory.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Game/BaseCharacter.h"

APowerBombItems::APowerBombItems()
{
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshAsset(TEXT("/Game/Material/Item/M_BonusPower.M_BonusPower"));
	if (MeshAsset.Succeeded())
	{
		ItemMesh->SetMaterial(0, MeshAsset.Object);
	}
	FItemsFactory::GetFactory()->RegisterNewItem(APowerBombItems::StaticClass());
}

void APowerBombItems::Consume(ABaseCharacter* InstigatorChar)
{
	InstigatorChar->AddBombsPower(1);
	Destroy();
}
