// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/BasePickableItems.h"

#include "Character/BaseCharacter.h"
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
	ABaseCharacter* tempCharacter = Cast<ABaseCharacter>(OtherActor);
	if (tempCharacter)
	{
		Consume(tempCharacter);
	}
}


