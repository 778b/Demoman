// Fill out your copyright notice in the Description page of Project Settings.

#include "Block/BaseBlock.h"

#include "Components/StaticMeshComponent.h"




ABaseBlock::ABaseBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = BlockMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/Block.Block"));
	if (MeshAsset.Succeeded())
	{
		BlockMesh->SetStaticMesh(MeshAsset.Object);
	}
	BlockMesh->SetCollisionProfileName("BlockAllDynamic");
}


void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

