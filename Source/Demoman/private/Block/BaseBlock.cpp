// Fill out your copyright notice in the Description page of Project Settings.

#include "Block/BaseBlock.h"

#include "ItemsFactory.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"




ABaseBlock::ABaseBlock()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = BlockMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/Block.Block"));
	if (MeshAsset.Succeeded())
	{
		BlockMesh->SetStaticMesh(MeshAsset.Object);
	}
	BlockMesh->SetCollisionProfileName("BlockAllDynamic");
}


void ABaseBlock::SetChanceToDrop(float newChance)
{
	if (newChance >= 0.f && newChance <= 100.f)
	{
		ChanceToDrop = newChance;
	}
	else check(false);
}

void ABaseBlock::DropItem_Implementation()
{
	float calculus = FMath::RandRange(0.f, 100.f);
	if (calculus < ChanceToDrop)
	{
		FItemsFactory::GetFactory()->CreateItemInLocation(GetWorld(), GetActorLocation());
	}
}

bool ABaseBlock::DamageActor()
{
	if (BlockHealth != -1)
	{
		--BlockHealth;
		Execute_DamageActorReplicated(this);
	}
	return false;
}

void ABaseBlock::DamageActorReplicated_Implementation()
{
	if (BlockHealth <= 0)
	{
		DropItem();
		Destroy();
	}
	else
	{
		// Visual damage block;
		//BlockMesh->GetMaterial(0);
	}
}


void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();
	
}


