// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Items/Public/BaseBlock.h"

#include "Components/StaticMeshComponent.h"




ABaseBlock::ABaseBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = BlockMesh;
}


void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

