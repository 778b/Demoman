// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePickableItems.h"

#include "Components/StaticMeshComponent.h"
#include "Character/BaseCharacter.h"




ABasePickableItems::ABasePickableItems()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemsMesh"));
	RootComponent = ItemMesh;

	ItemMesh->OnComponentBeginOverlap.AddDynamic(this, &ABasePickableItems::OnBeginOverlap);
}

void ABasePickableItems::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePickableItems::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABasePickableItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

